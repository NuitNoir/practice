import MeCab
# -*- coding:utf-8 -*- 

class parser:
  def __init__(self):
    global mecab 
    mecab = MeCab.Tagger('mecabrc')
    print 'construct parser'

  def tokenize(self, text):
    # 注意点。変数にエンコード済みテキストを渡す
    # 参照 http://shogo82148.github.io/blog/2012/12/15/mecab-python/
    encode_text = text.encode('utf-8')
    node = mecab.parseToNode(encode_text)
    while node:
      if node.feature.split(',')[0] == '名詞': 
        #node.decode('utf-8')
        #yield node.feature.split # .decode('utf-8')
        #print node.surface + '\t' + node.feature  
        yield node.surface
      node = node.next

  def get_words(contents):
    ret = []
    for k, content in contents.item():
      ret.append(get_words_main(content))
    return ret

  def get_words_main(self, content):
    #print content
    tokens = []
    
    #return [token for token in self.tokenize(content)]
    try:
      for token in self.tokenize(content):
        # token <-- yield node.surface 
        # print token
        tokens.append(token)
      #print tokens
      return tokens
    except (TypeError) as e:
      print(e)

if __name__ == '__main__':
  words = get_words({'FILE_NAME': 'CONTENT'})
  print(words)

class parser2:
  def __init__(self):
    print "cunstruct parser"

  def parse(unicode_string):
    tagger = MeCab.Tagger(MECAB_MODE)
    text = unicode_string.encode(PARSE_TEXT_ENCODING)
    node = tagger.parseToNode(text)

    words   = []
    nouns   = []
    verbs   = []
    adjs    = []
    while node:
      pos = node.feature.split(",")[0]
      word = node.surface.decode("utf-8")
      if pos == "名詞":
        print word
        nouns.append(word)
      elif pos == "動詞":
        verbs.append(word)
      elif pos == "形容詞":
        adjs.append(word)
      words.append(word)
      node = node.next
    parsed_words_dict = {
        "all": words[1:-1],
        "nouns": nouns,
        "verbs": verbs,
        "adjs": adjs
        }
    return parsed_words_dict




