# -*- coding:utf-8 -*-
__author__ = 'kyota'

import mysql.connector

from parse_mod import parser as pm
import re
MECAB_MODE = 'mecabrc'
PARSE_TEXT_ENCODING = 'utf-8'

class DictionaryMaker:
  def __init__(self, table, columns, where=" ", database="jeek_development"):
    self.table = table
    self.columns = columns
    self.where = where
    self.database = database

  def make_dictionary_from_mysql(self):
    try:
      rows = self.read_from_mysql()
      documents = self.get_documents(rows)
    except (mysql.connector.errors.ProgrammingError) as e:
      print (e)
    dictionary = self.make_dictionary(documents)
    return dictionary, documents, rows

  def make_dictionary(self, documents):
    from gensim import corpora
    dictionary = corpora.Dictionary(documents)
    dictionary.filter_extremes(no_below=10, no_above=0.3)
    dictionary.save_as_text("data/jeek_"+self.table+"_"+self.columns[0]+".dic")
    ########### 記号削除
    ###### 記号削除用正規表現
    pattern = re.compile("[<>().-=/]")
    # stop_ids = [dictionary.token2id[doc] for doc in dictionary.itervalues() if pattern.match(doc)]
    stop_ids = []
    for doc in dictionary.itervalues():
      if pattern.match(doc):
        print(doc)
        stop_ids.append(dictionary.token2id[doc])
    dictionary.filter_tokens(stop_ids) # remove stop words
    dictionary.compactify() # remove gaps in id sequence after words that were removed

    return dictionary

  def get_documents(self, rows):
    documents = []
    parser = pm()
    for row in rows:
        document = " ".join(row[1:])
        words = parser.get_words_main(document)
        documents.append(words)
        # print words
    return documents

  def read_from_mysql(self):
    cnn = mysql.connector.connect(host='localhost',
      port=3306,
      db= self.database,
      user='kyota')
    cur = cnn.cursor()

    columns = ", ".join(self.columns)
    print columns
    cur.execute(" select "+self.table+".id, "+columns+" from "+self.table+self.where)
    rows = cur.fetchall()
    return rows

def main():
  documents = []
  dm = DictionaryMaker("articles", ["image_comment_1", "image_comment_2"])
  dm.database = "jeek_development"
  dm.table = "articles"
  dm.make_dictionary_from_mysql()

######### Execute
if __name__ == "__main__":
  main()

