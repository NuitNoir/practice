# -*- coding:utf-8 -*- 

import mysql.connector
import sklearn

from ..parse_mod import parser as pm
from ..kmeans_cluster import kmeans_cluster as kc

# from parse_mod import parser
# ## Constants                                                                                                                             
MECAB_MODE = 'mecabrc'
PARSE_TEXT_ENCODING = 'utf-8'

def main():
  words = []
  documents = []
  parser = pm()
  try:
    cnn = mysql.connector.connect(host='localhost',
        port=3306,
        db="jeek_development",
        user='kyota',
        passwd='ciel6571')
    #      charset="UTF8")

    cur = cnn.cursor()

    cur.execute("""select distinct user_id, user_detail from user_details limit 100 """)
    rows = cur.fetchall()

    for row in rows:
      # print("%d %s" % (row[1], row[2]))
      words = parser.get_words_main(row[1])
      documents.append(words)
      # print words 
  except (mysql.connector.errors.ProgrammingError) as e:
    print (e)

  # 　辞書作成
  from gensim import corpora
  dictionary = corpora.Dictionary(documents)
  dictionary.filter_extremes(no_below=10, no_above=0.3)
  dictionary.save_as_text("jeek_es.txt")
  print(dictionary.token2id)

  # BoWの作成 
  from gensim import corpora, matutils
  # global vectors
  vectors = []
  for document in documents:
    tmp = dictionary.doc2bow(document)
    dense = list(matutils.corpus2dense([tmp], num_terms=len(dictionary)).T[0])
    vectors.append(dense)

   # LSI
  lsi_docs = {}
  num_group = 2
  lsi_model = gensim.models.LsiModel(bow_docs.values(), id2word=dct.load_from_text('jeek_es.txt'),
                                         num_topics=num_group)
  for name in names:
      vec = bow_docs[name]
      sparse = lsi_model[vec]
      dense = vec2dense(sparse, num_group)
      lsi_docs[name] = sparse
      print name, ":", dense
          
  print "\nTopics"
  print lsi_model.print_topics()
      
  # parser = parse_mod.parser()
  # words = parser.get_words_main(row[2])

# # Execute
if __name__ == "__main__":
  main()


