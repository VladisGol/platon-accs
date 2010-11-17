#!/usr/bin/python
# -*- coding: utf-8 -*-

import os ,sys
import codecs
import kinterbasdb as kdb
import time
__file__=sys.argv[0]

src_dir = 'dicts/src/Dicts'
dest_dir = 'dicts/converted'
lang_list={'rus':1,'eng':2,'deu':3}

#Сменим кодировку словаря
def dict2utf(input_file,output_file, src_charset, to_charset):
    text=codecs.open(input_file,'r',src_charset).read()
    codecs.open(output_file,'w',to_charset).write(text)

#Прочитаем секци. по строкам
def get_section_lines(file):
        section_lines_count = int(file.readline())
        for i in xrange(0, section_lines_count):
            yield file.readline()
#Запишем словарь в БД
def dict2db(lang,mydb):
    dir = 'dicts/converted/'+lang
    morph_dict=os.path.normpath(os.path.join(os.path.dirname(__file__),dir,'morphs.utf8.mrd'))
    gramtab_dict=os.path.normpath(os.path.join(os.path.dirname(__file__),dir,'gramtab.utf8.mrd'))
    dict_file = codecs.open(gramtab_dict, 'r', 'utf8')
    #mydb=db.connect(dsn='platon_svn',user='SYSDBA', password='masterkey',dialect=3, charset='UTF8' )
    gramtab2db(dict_file,mydb,lang_list[lang])
    print 'Complit gramtab'
    dict_file.close()
    dict_file = codecs.open(morph_dict, 'r', 'utf8')
    rules2db(dict_file,mydb,lang_list[lang])
    print 'Complit save rules'
    accents2db(dict_file,mydb,lang_list[lang])
    print 'Complit save accents'
    pass_user_sessions(dict_file)
    print 'Complit save pass user logs'
    prefix2db(dict_file,mydb,lang_list[lang])
    print 'Complit save prefixes'
    lemma2db(dict_file,mydb,lang_list[lang])
    print 'Complit save lemma'

def gramtab2db(file,db,lang_id):
    SQL = "insert into INDEXER_ANCODE(ANCODE,ABBR,PART_OF_SPEECH,GRAMEMS,ID_LANG) values (?,?,?,?,?) "
    cur = db.cursor()
    for line in file:
            line=line.strip()
            if line.startswith('//') or line == '':
                continue
            g = line.split()
            if len(g)==3:
                g.append('')
            ancode, letter, type, info = g[0:4]
            #print (ancode,letter,type,info,lang_id)
            try:
                cur.execute(SQL ,(ancode,letter,type,info,lang_id))
            except:
                print (SQL,ancode,letter,type,info,lang_id)
    db.commit()            

def rules2db(file,db,lang_id):
        SQL = "insert into INDEXER_RULES(NUM,POS,ID_LANG,SUFFIX,ANCODE,PREFIX,INFO) values (?,?,?,?,?,?,?) "
        #SQL_PREF="insert into INDEXER_POSSIBLE_PREFIXES(NUM,POS,PREFIX,ID_LANG) values (?,?,?,?) "
        cur = db.cursor()
        i=0
        for line in get_section_lines(file):
            line_rules = line.strip().split('%')
            j=0
            for rule in line_rules:
                if not rule:
                    continue
                parts = rule.split('*') #parts: suffix, ancode, prefix
                if len(parts)==2:
                    parts.append('')
                #inf=''
                '''if len(parts[1])>2:
                    inf=parts[1][2:]
                    parts[1]=parts[1][:2]
                    print  inf,parts[1],parts
                parts=[i,j,lang_id]+parts
                parts.append(inf)'''
                cur.execute(SQL ,tuple([i,j,lang_id,parts[0],parts[1][:2],parts[2],parts[1][2:]]))
    
                #if parts[2]:
                    #cur.execute(SQL_PREF ,(i,j,parts[-1],lang_id))
                j+=1
            i=i+1
            db.commit()

def accents2db(file,db,lang_id): 
        SQL = "insert into INDEXER_ACCENTS(NUM,POS,ACCENT,ID_LANG) values (?,?,?,?) "
        cur = db.cursor()
        i=0
        for line in get_section_lines(file):
            line_accent = line.strip().split(';')
            j=0
            for accent in line_accent:
                if not accent:
                    continue
                parts=[i,j,accent,lang_id]
                cur.execute(SQL ,tuple(parts))
                j+=1
            i=i+1
            db.commit()
def pass_user_sessions(file):
    for line in get_section_lines(file): pass

def prefix2db(file,db,lang_id): 
        SQL = "insert into INDEXER_PREFIXES(NUM,PREFIXES,ID_LANG) values (?,?,?) "
        cur = db.cursor()
        i=0
        for line in get_section_lines(file):
            parts=[i,line,lang_id]
            cur.execute(SQL ,tuple(parts))                
            i=i+1
        db.commit()

def lemma2db(file,db,lang_id):
    SQL = "execute procedure SAVE_LEMMA(?,?,?,?,?,?) "
    cur = db.cursor()
    print 'Save lemma \n'
    for line in get_section_lines(file):
        record = line.split()
        if record[5]=='-':
            record[5]=-1
        if record[4]=='-':
            record[4]=''
        base, id_rule,accent,ancode,prefix = record[0], record[1],record[2],record[4],record[5]
        #print base,id_rule
        cur.execute(SQL ,( id_rule,accent,ancode,prefix,lang_id,base))  
        db.commit()

def run(mydb):
    try:
        print time.asctime()
        print "encoding english.."
        dict2utf(os.path.normpath(os.path.join(os.path.dirname(__file__),src_dir,'SrcMorph/EngSrc/morphs.mrd')),
                     os.path.normpath(os.path.join(os.path.dirname(__file__),dest_dir,'eng/morphs.utf8.mrd')), 'latin1', 'utf8')
        dict2utf(os.path.normpath(os.path.join(os.path.dirname(__file__),src_dir,'Morph/egramtab.tab')),
                     os.path.normpath(os.path.join(os.path.dirname(__file__),dest_dir,'eng/gramtab.utf8.mrd')), 'latin1', 'utf8')

        dict2db('eng',mydb)
        print time.asctime()
        print "encoding russian.."
        dict2utf(os.path.normpath(os.path.join(os.path.dirname(__file__),src_dir,'SrcMorph/RusSrc/morphs.mrd')),
                     os.path.normpath(os.path.join(os.path.dirname(__file__),dest_dir,'rus/morphs.utf8.mrd')), 'cp1251', 'utf8')
        dict2utf(os.path.normpath(os.path.join(os.path.dirname(__file__),src_dir,'Morph/rgramtab.tab')),
                     os.path.normpath(os.path.join(os.path.dirname(__file__),dest_dir,'rus/gramtab.utf8.mrd')), 'cp1251', 'utf8')
        dict2db('rus',mydb)
        print time.asctime()
        print "encoding deutch.."
        dict2utf(os.path.normpath(os.path.join(os.path.dirname(__file__),src_dir,'SrcMorph/GerSrc/morphs.mrd')),
                     os.path.normpath(os.path.join(os.path.dirname(__file__),dest_dir,'deu/morphs.utf8.mrd')), 'cp1252', 'utf8')
        dict2utf(os.path.normpath(os.path.join(os.path.dirname(__file__),src_dir,'Morph/ggramtab.tab')),
                     os.path.normpath(os.path.join(os.path.dirname(__file__),dest_dir,'deu/gramtab.utf8.mrd')), 'cp1252', 'utf8')
            
        dict2db('deu',mydb)
        print time.asctime()
        print "done."
        return 1
    except:
        print "Error in save dictionary to db"
        return 0
        

if __name__ == '__main__':
    run()
    

    


