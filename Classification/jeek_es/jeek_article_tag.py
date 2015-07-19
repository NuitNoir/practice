# -*- coding:utf-8 -*-
from dictionary_maker import DictionaryMaker as DM
import numpy as np

def main():
  dm = DM("articles", ["hiring_text"])
  # dm.columns = ["image_comment_1", "image_comment_2", "image_comment_3"]
  dm.where = """ left outer join article_job_types on articles.id = article_job_types.article_id
                 left outer join job_types on article_job_types.job_type_id = job_types.id
                 where job_types.id = 6
             """
  dictionary, documents, rows = dm.make_dictionary_from_mysql()
  print documents[0]

  ######### LSI
  from gensim import corpora, similarities
  lsi = LSI(documents, dictionary, topic_num=50)
  # dictionary_name = "data/jeek_"+dm.table+"_"+dm.columns[0]+".dic"
  # dictionary = corpora.dictionary.loadFromText(dictionary_name)
  ######## making lsi model and corpus
  lsi_corpus, lsi_model =lsi.make_lsi()

  ####### article major topic
  article_topics = np.ndarray((0,3))
  # for i in range(1, len(lsi_corpus)):
  i = -1
  f = open("data/article_topic.dat", "w")
  for corpus in lsi_corpus:
    i += 1
    for tuple in corpus:
      if (tuple[1] > 0.2) :
        f.write("id="+str(rows[i][0])+"\ttopic_id="+str(tuple[0]+1)+"\tval="+str(tuple[1])+"\n")
        article_topics = np.concatenate((article_topics, [[rows[i][0], tuple[0], tuple[1]]]))
  f.close()

  # file = open("data/lsi_topics.text", "w")
  for i in range(1, 50):
    print(lsi_model.print_topic(i))
    # file.writelines(lsi_model.print_topic(i))
  # file.close()

  # lsi.make_similarity(rows)

##### attributes: documents, dictionary, lsi_corpus, lsi_model
class LSI:
  def __init__(self, documents, dictionary, topic_num=100):
    self.documents = documents
    self.dictionary = dictionary
    self.topic_num = topic_num

  def make_lsi(self):
    from gensim import corpora, models
    bow_corpus = [self.dictionary.doc2bow(text) for text in self.documents]
    corpora.MmCorpus.serialize('data/article_image_comment_1.mm', bow_corpus)
    # corpus = corpora.MmCorpus('data/article_image_comment_1.mm')
    tfidf_model = models.TfidfModel(bow_corpus) # step 1 -- initialize a model
    tfidf_corpus = tfidf_model[bow_corpus]
    corpora.MmCorpus.serialize('data/article_image_comment_1_tfidf.mm', tfidf_corpus)
    self.lsi_model = models.LsiModel(tfidf_corpus, id2word=self.dictionary, num_topics=self.topic_num)
    self.lsi_model.save("data/lsi.model")
    self.lsi_corpus = self.lsi_model[tfidf_corpus]
    return self.lsi_corpus, self.lsi_model

  def make_similarity(self, rows):
    ######## similarities
    from gensim import similarities
    index = similarities.MatrixSimilarity(self.lsi_model[self.lsi_corpus])
    index.save('data/jeek_article_image_comment.index')
    sims = index[self.lsi_corpus]
    similarities = np.ndarray((1, 3))
    for id1 in range(0, len(sims)):
      for id2 in range(0, len(sims[id1])):
        val=sims[id1][id2]
        if val > 0.2:
          # print("id1="+str(rows[id1][0])+"\tid2="+str(rows[id2][0])+"\tval="+str(val))
          similarities = np.concatenate((similarities, [[rows[id1][0], rows[id2][0], int(val)]]), axis=0)
    # print(list(enumerate(sims)))

    # sims = sorted(enumerate(sims), key=lambda item: -item[1])
    # print(sims)
    return similarities, self.lsi_corpus, self.lsi_model


######### Execute
if __name__ == "__main__":
  main()

