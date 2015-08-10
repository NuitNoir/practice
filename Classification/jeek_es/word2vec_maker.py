# -*- coding:utf-8 -*-
__author__ = 'kyota'

import mysql.connector
from gensim import models
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
    self.documents = []

  def make_word2vec_from_mysql(self):
    try:
      rows = self.read_from_mysql()
      documents = self.get_documents()
    except (mysql.connector.errors.ProgrammingError) as e:
      print (e)
    word2vec = self.make_word2vec()
    return word2vec, documents, rows

  def make_doc2vec_from_mysql(self):
    try:
      rows = self.read_from_mysql()
      documents = self.get_documents()
    except (mysql.connector.errors.ProgrammingError) as e:
      print (e)
    doc2vec = self.make_doc2vec()
    return doc2vec, documents, rows

  def make_doc2vec(self):
    from gensim import models
    doc2vec_model = models.Doc2Vec(self.documents, size=30, window=4, min_count=5, workers=4)
    doc2vec_model.save("data_word2vec/"+self.table+"_"+self.columns[0]+"_doc_model.txt")
    return doc2vec_model

  def make_word2vec(self):
    from gensim import models
    word2vec_model = models.Word2Vec(self.documents, size=50, window=5, min_count=5, workers=4)
    word2vec_model.save("data_word2vec/"+self.table+"_"+self.columns[0]+"_word_model.txt")
    # word2vec_model = models.Word2Vec.load("data/word2vec/model.text")  ### read word2vec_model
    # print (word2vec_model.most_similar(positive=['意欲']))
    return word2vec_model

  def get_documents(self):
    parser = pm()
    self.documents = []
    for row in self.rows:
        # document = " ".join(row[1:])
        for document in row[1:]:  ## document is each column (cf: in dictionary document is each article)
          words = parser.get_words_main(document)
          self.documents.append(words)
          # print words
    return self.documents

  def read_from_mysql(self):
    cnn = mysql.connector.connect(host='localhost',
      port=3306,
      db= self.database,
      user='kyota')
    cur = cnn.cursor()

    columns = ", ".join(self.columns)
    print columns
    query = " select "+self.table+".id, "+columns+" from "+self.table+self.where
    print query
    cur.execute(query)
    self.rows = cur.fetchall()
    return self.rows

def main():
  # dm = Word2VecMaker("news", ["title"], database="pocket_career_development")
  # dm.make_word2vec_from_mysql()
  word2vec = models.Word2Vec.load("data_word2vec/news_title_word_model.txt")

  keywords = ['Apple', '情熱', '思考', '感情']
  f = open("data_word2vec/similarity.txt", "w")
  for keyword in keywords:
    # print (word2vec.most_similar(positive=[keyword]))
    f.write(keyword + "\n")
    # for word in word2vec.most_similar(positive=[keyword], topn=20):
    for word in word2vec.most_similar(positive=[keyword], topn=20):
      f.write(word[0] +"  \t"+ str(word[1]) + "\n")
    f.write("\n")
  f.close()

  #################################
  # dm = Word2VecMaker("", [''])
  # # dm.columns = ["image_comment_1", "image_comment_2", "image_comment_3"]
  # dm.columns = ['hiring_text']
  # # dm.columns = ['user_detail']
  # dm.database = "jeek_development"
  # dm.table = "articles"
  # # dm.table = "user_details"
  # ##############################
  # word2vec, documents, rows = dm.make_word2vec_from_mysql()
  # keywords = ['']
  # # keywords = ['論理', '情熱', '思考', '感情']
  # f = open("data_word2vec/similarity.txt", "w")
  # for keyword in keywords:
  #   # print (word2vec.most_similar(positive=[keyword]))
  #   f.write(keyword + "\n")
  #   # for word in word2vec.most_similar(positive=[keyword], topn=20):
  #   for word in word2vec.most_similar(positive=['実行'], topn=20):
  #     f.write(word[0] +"  \t"+ str(word[1]) + "\n")
  #   f.write("\n")
  # f.close()
  ##############################
  # doc2vec, documents, rows = dm.make_doc2vec_from_mysql()
  # f = open("data_word2vec/doc_similarity.txt", "w")
  # for document in documents[0:10]:
  #   # print (word2vec.most_similar(positive=[keyword]))
  #   f.write(document + "\n")
  #   for word in doc2vec.most_similar(positive=[document], topn=15):
  #     f.write(word[0] +"  \t"+ str(word[1]) + "\n")
  #   f.write("\n")
  # f.close()
  ##########################

######### Execute
if __name__ == "__main__":
  main()

