#!/usr/bin/python
# -*- coding: utf-8 -*-
# author: Sergei Borisenkov

import os ,sys
import kinterbasdb as kdb
import codecs,re
from statemachine import StateMachine

__file__=sys.argv[0]

class NoCreate(Exception): pass
	
class ParserScript:
	def __init__(self,db,file_name,log_file_name=None):
		self.sm = StateMachine()
		self.db=db
		self.cur = self.db.cursor()
		file=codecs.open(file_name,'r','utf8')
		if log_file_name: self.log=codecs.open(log_file_name,'w','utf8')
		self.line=self.get_script_line(file)
		self.SQL=u''
		self.r=re.compile('SET[\\s]+TERM')
		self.ex_term=';'
		self.create_state(self.sm)
		self.State=''
		self.log=None
		print "Parse script class"
		
	def run(self):
		self.sm.run(['','READ_LINE'])        

	def get_script_line(self,file):
		while 1:
			str=file.readline()
			if not str: break
			yield str

	def get_line(self,val):         
		if len(val[0].strip())==0:
			newState='CLEAN_REM'
			val[1]=newState
		else:
			newState=val[1]
		try:
			s=self.line.next()
			val[0]+=s
		except:
			newState='COMMIT'
		if newState<>'COMMIT' and len(re.sub('\\s+','',val[0]).strip())==0 :
			newState='READ_LINE'
		return (newState, val)

	def clean_leades_rem(self,val): 
		newState='EX_TERM'
		if val[0].strip()[:2]=='--':
			val[0]=''
			newState='READ_LINE'
		if val[0].strip()[:2]=='/*':
			pos=val[0].find('*/')
			if pos==-1:
				newState='READ_LINE'
			else:
				val[0]=re.sub('\\s+','',val[0][pos+2:]) 
				if len(val[0].strip())==0:
					newState='READ_LINE'
		val[1]=newState
		return (newState, val)

	def get_term(self,val): 
		pos_term=val[0].find(self.ex_term)
		if pos_term==-1:
				newState='READ_LINE'
				val[1]='TERM'
		else:
			self.SQL+=val[0][:pos_term]
			newState='EXEC_SQL'
			val[0]=val[0][pos_term+1:]
		return (newState, val)
		
	def get_ex_term(self,val):
		#print 'EX_TERM>> '+val[0]
		pos_ex_term=self.r.search(val[0])
		newState='EXT_TERM'
		if pos_ex_term==None: 
			newState='TERM'
		elif val[0][:pos_ex_term.start()].strip()=='':
			pos=val[0].find(self.ex_term)
			if pos==-1:
				val[1]=newState
				newState='READ_LINE'
			else:
				self.ex_term=re.sub('SET[\\s]+TERM','',val[0][pos_ex_term.start():pos]).strip()
				val[1]=newState
				newState='READ_LINE'
				val[0]=val[0][pos+1:]
		else:
			if val[0][:pos_ex_term].find(';')<>-1:
				val[1]=newState
				newState='TERM'
		return (newState, val)

	def exec_SQL(self,val):
		#print self.SQL+'<<\n'
		if self.log:
			self.log.write(self.SQL)
			self.log.write('<<\n')
		try:
			self.cur.execute(self.SQL)
		except kdb.ProgrammingError,e:
			if e[0]<>-607:
				newState='ERROR'
		self.SQL=''
		newState=val[1]
		return (newState, val)

	def commit(self,val):
		self.db.commit() 
		print 'COMMIT'
		self.State='COMPLIT'
		newState='END'
		return (newState, val)
		
	def error(self,val):
		self.db.rollback()
		print 'ERROR'
		self.State='ERROR'
		newState='End'
		return (newState, val)
		
	def create_state(self,m):
		m.add_state('READ_LINE', self.get_line) #, 'TERM','EXT_TERM','COMMIT','SKIP'
		m.add_state('CLEAN_REM', self.clean_leades_rem)
		m.add_state('TERM', self.get_term)
		m.add_state('EX_TERM', self.get_ex_term)
		m.add_state('EXEC_SQL',self.exec_SQL)
		m.add_state('COMMIT',self.commit)
		m.add_state('ERROR',self.error)
		m.add_state('END',None,end_state=1)
		m.set_start('READ_LINE')
        

if __name__== "__main__":
	mydb=kdb.connect(dsn='platon_svn',user='SYSDBA', password='masterkey',dialect=3, charset='UTF8' )
	input_file=os.path.dirname(__file__)+'\\indexer.sql'
	output_file=os.path.dirname(__file__)+'\\out.sql'
	parser=ParserScript(mydb,input_file,output_file)
	parser.run()
	print parser.State
