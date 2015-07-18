# -*- coding:utf-8 -*-
from dictionary_maker import DictionaryMaker as DM

def main():
  dm = DM("articles", ["image_comment_1", "image_comment_2", "image_comment_3"])
  dm.where = """ left outer join article_job_types on articles.id = article_job_types.article_id
                 left outer join job_types on article_job_types.job_type_id = job_types.id
                 where job_types.id = 6
             """
  dictionary, documents = dm.make_dictionary_from_mysql()
  print documents[0]

  ######### LSI
  from gensim import corpora
  lsi = LSI()
  dictionary_name = "data/jeek_"+dm.table+"_"+dm.columns[0]+".dic"
  # dictionary = corpora.dictionary.loadFromText(dictionary_name)
  lsi_model =lsi.make_lsi(documents, dictionary)
  file = open("data/lsi_topics.text", "w")
  for i in range(1, 101):
    print(lsi_model.print_topic(i))
    # file.writelines(lsi_model.print_topic(i))
  file.close()

class LSI:
  def make_lsi(self, documents, dictionary):
    from gensim import corpora, models
    bow_corpus = [dictionary.doc2bow(text) for text in documents]
    corpora.MmCorpus.serialize('data/article_image_comment_1.mm', bow_corpus)
    # corpus = corpora.MmCorpus('data/article_image_comment_1.mm')
    tfidf_model = models.TfidfModel(bow_corpus) # step 1 -- initialize a model
    tfidf_corpus = tfidf_model[bow_corpus]
    lsi_model = models.LsiModel(tfidf_corpus, id2word=dictionary, num_topics=100)
    lsi_model.save("data/lsi.model")
    return lsi_model

######### Execute
if __name__ == "__main__":
  main()

