# -*- coding:utf-8 -*-
__author__ = 'kyota'

import mysql.connector

from parse_mod import parser as pm
import re
MECAB_MODE = 'mecabrc'
PARSE_TEXT_ENCODING = 'utf-8'

class Word2VecMaker:
  def __init__(self, table, columns, where=" ", database="jeek_development"):
    self.table = table
    self.columns = columns
    self.where = where
    self.database = database

  def make_word2vec_from_mysql(self):
    try:
      rows = self.read_from_mysql()
      documents = self.get_documents(rows)
    except (mysql.connector.errors.ProgrammingError) as e:
      print (e)
    word2vec = self.make_word2vec(documents)
    return word2vec, documents, rows

  def make_word2vec(self, documents):
    from gensim import models
    word2vec_model = models.Word2Vec(documents, size=100, window=5, min_count=5, workers=4)
    word2vec_model.save("data_word2vec/model.txt")
    # word2vec_model = models.Word2Vec.load("data/word2vec/model.text")  ### read word2vec_model
    # print (word2vec_model.most_similar(positive=['意欲']))
    return word2vec_model

  def get_documents(self, rows):
    documents = []
    parser = pm()
    for row in rows:
        # document = " ".join(row[1:])
        for document in row[1:]:  ## document is each column (cf: in dictionary document is each article)
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
  dm = Word2VecMaker("articles", ['hiring_text'])
  # dm.columns = ["image_comment_1", "image_comment_2", "image_comment_3"]
  dm.columns = ['hiring_text']
  dm.database = "jeek_development"
  dm.table = "articles"
  word2vec, documents, rows = dm.make_word2vec_from_mysql()
  # keywords = ['意欲', 'プログラミング', '英語', '事業', '大学', 'マーケティング', 'エンジニア']
  keywords = ['エンジニア']
  f = open("data_word2vec/similarity.txt", "w")
  for keyword in keywords:
    # print (word2vec.most_similar(positive=[keyword]))
    f.write(keyword + "\n")
    for word in word2vec.most_similar(positive=[keyword], negative=['独学']):
      f.write(word[0] +"  \t"+ str(word[1]) + "\n")
    f.write("\n")
  f.close()


######### Execute
if __name__ == "__main__":
  main()

