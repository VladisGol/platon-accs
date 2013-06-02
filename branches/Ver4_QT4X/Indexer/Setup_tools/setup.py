#!/usr/bin/python
# -*- coding: utf-8 -*-

import os ,sys
import codecs
import kinterbasdb as kdb
from statemachine import StateMachine

__file__=sys.argv[0]
sys_name=os.name

class ParserCmdLine:
	def __init__(self,argv):
		#self.argv=argv
		self.params={'-h':'localhost','-d':'','-s':'','-u':'SYSDBA','-p':'masterkey'}
		self.ch='UTF8'
		self.ch=self.detect_codecs()
		self.mydb=None
		self.cmd=None
		self.sm = StateMachine()
		self.create_state(self.sm)
		self.parse_cmd(argv)
		

	def bad_parameters(self):
		print u'Ошибка при вводе параметров'
		self.help()

	def help(self):
		print u'''Использование: 
setup.py i [-h host] [-d database] [-s dsn] [-u user] [-p password] 
setup.py c [-h host] [-d database] [-s dsn] [-u user] [-p password] 
Комманды:
	i	- создать в БД структуры Индексатора, 
			требует файл indexator.sql
	c	- удалить из БД структуры Индексатора
	u	- обновить в БД структуры Индексатора, 
			требует файл update_xxxx.sql
Параметры:
	-h host 	- имя сервера БД
	-d database - алиас или путь к базе данных на сервере
	-s dsn 		- источник данных в формате host:database
	-u user 	- имя пользователя (по умолчанию SYSDBA)
	-p password - пароль (по умолчанию masterkey)
		'''.encode(self.ch)
		
	def no_connect(self):
		print u"Ошибка подключения! Проверте параметры подключения и доступность сервера.".encode(self.ch)   
		self.cmd=''

		
	def connect_db(self):
		try:
			if (self.params['-s']==''):# and self.params['-d']!=''):
				print self.params
				self.mydb=kdb.connect(host=self.params['-h'],database=self.params['-d'],user=self.params['-u'], password=self.params['-p'],dialect=3, charset='UTF8' )
			else:
				self.mydb=kdb.connect(dsn=self.params['-s'],user=self.params['-u'], password=self.params['-p'],dialect=3, charset='UTF8' )
			return self.mydb
		except:
			print 'Except'
			self.no_connect()
			return None

	def detect_codecs(self):
		if sys_name=='nt':
			return 'CP866'
		else:
			return 'UTF8'
	
	def parse_cmd(self,argv):
		is_parse=1
		i=0
		cmd=''
		print len(argv)
		if len(argv)>1:
			try:
				while i<len(argv):
					print argv[i],i,argv[i] in ['i','c','u'] 
					if argv[i] in ['i','c','u']:
						cmd=argv[i]
						print cmd
						i+=1
					elif self.params.has_key(argv[i]):
						self.params[argv[i]]=argv[i+1]
						print argv[i+1]
						i+=2
					else:
						print i
						self.bad_parameters()
						is_parse=0
						break
				print self.params
			except:
				self.bad_parameters()
				is_parse=0
			else: 
				if is_parse: 
					self.cmd=cmd
					self.mydb=self.connect_db()
		else:
			self.help()


	def create_state(self,m):
			m.add_state('c', self.clean_db) #, 'TERM','EXT_TERM','COMMIT','SKIP'
			m.add_state('cid', self.clean_db) #clean after install
			m.add_state('id', self.create_db_structures)
			m.add_state('d', self.create_dictionares)
			m.add_state('u',self.update_db)
			m.add_state('ERROR',self.error)
			m.add_state('',None,end_state=1)
			
	def run_command(self):
		if self.cmd:
			if self.cmd=='i': 
				cmd='cid'
			else:
				cmd=self.cmd
			self.sm.set_start(cmd)
			self.sm.run(cmd)
		
	
	def clean_db(self,val):
		import clean_db
		CC=clean_db.IndexerClean()
		if CC.clean(self.mydb)==1:
			newState=val[1:]
		else:
			newState='ERROR'
		return (newState, newState)
		
	def create_db_structures(self,val):
		from parse_script import ParserScript
		input_file=os.path.dirname(__file__)+'\\indexer.sql'
		#output_file=os.path.dirname(__file__)+'\\out.sql'
		parser=ParserScript(self.mydb,input_file)
		parser.run()
		if parser.State=='COMPLIT':
			newState=val[1:]
		else:
			newState='ERROR'
			print "Error in create structures"
		return (newState, newState)
		
	def create_dictionares(self,val):
		import dict_encode
		if dict_encode.run(self.mydb)==1:
			newState=val[1:]
		else:
			newState='ERROR'
			print "Error in create dictionaries"
		return (newState, newState)
	
	def update_db(self,val): 
		return('','')
	
	def error(self,val):
		print 'ERROR in state '+val
		newState=''
		return (newState, newState)

if __name__ == '__main__':
	parser=ParserCmdLine(sys.argv[1:])
	parser.run_command()

