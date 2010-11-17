#!/usr/bin/python
# -*- coding: utf-8 -*-
# author: Sergei Borisenkov


import os ,sys
import kinterbasdb as kdb
import re

class NoClean(Exception): pass

class IndexerClean:
	def __init__(self):
		self.struct=['FOREIGN KEY','PRIMARY KEY','TRIGGER','INDEX','TABLE','SEQUENCE']
		self.depend_struct={'FOREIGN KEY':'ALTER TABLE {0} DROP CONSTRAINT {1};', 
			'PRIMARY KEY':'ALTER TABLE {0} DROP CONSTRAINT {1};', 
			'TRIGGER': 'DROP TRIGGER {1}',
			'INDEX':'DROP INDEX {1}',
			'TABLE': 'DROP TABLE {1}',
			'SEQUENCE':'DROP SEQUENCE {1};'}
		self.nondepend_struct={'PROCEDURE':['DROP PROCEDURE GET_ALLOWED_FORMS;', 'DROP PROCEDURE SAVE_LEMMA;']}
		self.SQLs={ 'FOREIGN KEY':"SELECT RDB$CONSTRAINT_NAME, RDB$CONSTRAINT_TYPE, RDB$INDEX_NAME FROM RDB$RELATION_CONSTRAINTS WHERE RDB$RELATION_NAME = '{0}' and RDB$CONSTRAINT_TYPE like 'FOREIGN KEY';", 'PRIMARY KEY':"SELECT RDB$CONSTRAINT_NAME, RDB$CONSTRAINT_TYPE, RDB$INDEX_NAME FROM RDB$RELATION_CONSTRAINTS WHERE RDB$RELATION_NAME = '{0}' and RDB$CONSTRAINT_TYPE like 'PRIMARY KEY';", 'TRIGGER':"SELECT RDB$TRIGGER_NAME FROM RDB$TRIGGERS WHERE ((RDB$SYSTEM_FLAG = 0) OR (RDB$SYSTEM_FLAG IS NULL)) AND (RDB$TRIGGER_NAME NOT IN (SELECT RDB$TRIGGER_NAME FROM RDB$CHECK_CONSTRAINTS)) and RDB$RELATION_NAME = '{0}';", 'INDEX':"SELECT RDB$INDEX_NAME FROM RDB$INDICES WHERE RDB$RELATION_NAME = '{0}';", 'SEQUENCE':"SELECT RDB$GENERATOR_NAME FROM RDB$GENERATORS WHERE ((RDB$SYSTEM_FLAG = 0) OR (RDB$SYSTEM_FLAG IS NULL)) and RDB$GENERATOR_NAME like '%%INDEXER%%';", 'TABLE': "select rdb$relation_name tbl_name from rdb$relations where rdb$system_flag = 0 and (rdb$relation_name like 'INDEXER%%' ) order by rdb$relation_name asc;"}


	def list_table(self,mydb):
		cur = mydb.cursor()
		cur.execute('''select rdb$relation_name tbl_name from rdb$relations where rdb$system_flag = 0 and (rdb$relation_name like 'INDEXER%' ) order by rdb$relation_name asc;''')
		lst=[]
		for i in cur:
			lst.append(i[0])
		return lst

	def delete_nondepend_struct(self,mydb):#,nondepend_struct):
		cur = mydb.cursor()
		for s in self.nondepend_struct:
			for sql in self.nondepend_struct[s]:
				try:
					cur.execute(sql)
				except kdb.ProgrammingError,e:
					if e[0]<>-607: raise NoClean("Not clean indexer db structures" )
				#print(sql)
		print "Delete non depend structures"

	def delete_depend_struct(self,mydb, tables):
		cur = mydb.cursor()
		cur2 = mydb.cursor()
		for s in self.struct:
			for t in tables:
				print(self.SQLs[s].format(t.strip()))
				cur.execute(self.SQLs[s].format(t.strip()))
				for d in cur:
					#print(d[0],depend_struct[s].format(t.strip(),d[0].strip()))
					try:
						cur2.execute(self.depend_struct[s].format(t.strip(),d[0].strip()))
					except kdb.ProgrammingError,e:
						if e[0]<>-607: raise NoClean("Not clean indexer db structures" )
		print "Delete depend structures"

	def clean(self,mydb):
		try:
			print mydb
			self.delete_nondepend_struct(mydb)
			self.delete_depend_struct(mydb,self.list_table(mydb))
			mydb.commit()
			print "Complit delete"
			return 1
		except NoClean:
			mydb.rollback()
			print "Error to delete"
			return 0
		
	
if __name__ == '__main__':
	mydb=kdb.connect(dsn='platon_svn',user='SYSDBA', password='masterkey',dialect=3, charset='UTF8' )
	c=IndexerClean()
	try:
		c.clean(mydb)
		mydb.commit()
	except NoClean:
		mydb.rollback()
	print "Complit delete"