#!/usr/bin/env python
#  -*- coding:utf-8 -*-
#
# ~/.../csv_to_mongo.py
#

import os
import re
import pymongo
from bson.objectid import ObjectId
import csv

class FileSeeker:
    " ディレクトリ内のファイルを探しだすためのクラス "
    def __init__(self, ext="csv"):
        self.filenames = []
        self.pattern = re.compile(".*" + ext)

    def seek_dir(self, dirname):
        " dirname で渡されたディレクトリ以下から self.ext に一致するファイル名を返す. "
        if (not os.path.isdir(dirname)):
            if (self.pattern.match(dirname)):
                self.filenames.append(dirname)
            return dirname
        dirname = dirname + '/'
        filenames = os.listdir(dirname)
        for filename in filenames:
            self.seek_dir(dirname + filename)

class FileReader:
    " ファイル読み込みを行うクラス "
    def __init__(self):
        self.dicts = []

    def shiftjis_to_utf8(self, strs):
        " shift-jis からutf-8への変換を行う. 変換に失敗する文字を弾いたりもしている. "
        data = []
        for str in strs:
            val = ""
            try:
                str = str.strip('.')
                val = unicode(str, 'shift-jis').encode('utf-8')
            except UnicodeDecodeError as e:
                print(e)
            data.append(val)
        return data

    def file_to_dict(self, filenames):
        " ファイルの中身をdict型にして返す. １行目のデータをキー名, ２行目以降を値として保存する. 数値にしたいデータの定義がハードコードなのでそれも引数で受け取れるようにすると汎用性が出る. "
        for filename in filenames:
            with open(filename, encoding='shift-jis') as csvfile:
                reader = csv.reader(csvfile, delimiter=',')
                # reader.next()
                count = 0
                for row in reader:
                    data = []
                    data = self.shiftjis_to_utf8(row)
                    if (count == 0): ## 0行目のカラム名があれば
                        keys = data
                    else:
                        dic = dict(zip(keys, data))
                        self.dicts.append(dic)
                        ### 数値に変換。この部分ちょっとやばい
                        ### float に変換
                        if (dic.get("専有面積") != None and dic.get("専有面積") != ""):
                            dic["専有面積"] = float(dic["専有面積"])
                        if (dic.get("平米単価") != None and dic.get("平米単価") != ""):
                            dic["平米単価"] = float(dic["平米単価"])

                        ### int に変換
                        if (dic.get("更新料")  != None and dic.get("更新料") != ""):
                            dic["更新料"] = int(float(dic["更新料"]))
                        if (dic.get("賃料")   != None and dic.get("賃料") != ""):
                            dic["賃料"] = int(float(dic["賃料"]))
                        if (dic.get("築年月")  != None and dic.get("築年月") != ""):
                            dic["築年月"] = int(float(dic["築年月"]))
                        if (dic.get("徒歩分数") != None and dic.get("徒歩分数") != ""):
                            dic["徒歩分数"] = int(float(dic["徒歩分数"]))
                        if (dic.get("所在階")  != None and dic.get("所在階") != ""):
                            dic["所在階"] = int(float(dic["所在階"]))
                        if (dic.get("AT")   != None and dic.get("AT") != ""):
                            dic["AT"] = int(float(dic["AT"]))
                        if (dic.get("EV")   != None and dic.get("EV") != ""):
                            dic["EV"] = int(float(dic["EV"]))
                        if (dic.get("取得日")  != None and dic.get("取得日") != ""):
                            dic["取得日"] = int(float(dic["取得日"]))
                        if (dic.get("変更日")  != None and dic.get("変更日") != ""):
                            dic["変更日"] = int(float(dic["変更日"]))
                        if (dic.get("管理費")  != None and dic.get("管理費") != ""):
                            dic["管理費"] = int(float(dic["管理費"]))
                        if (dic.get("BT別")  != None and dic.get("BT別") != ""):
                            dic["BT別"] = int(float(dic["BT別"]))
                        if (dic.get("礼金")   != None and dic.get("礼金") != ""):
                            dic["礼金"] = int(float(dic["礼金"]))
                        if (dic.get("敷金")   != None and dic.get("敷金") != ""):
                            dic["敷金"] = int(float(dic["敷金"]))
                        if (dic.get("礼金")   != None and dic.get("礼金") != ""):
                            dic["礼金"] = int(float(dic["礼金"]))
                        if (dic.get("礼金")   != None and dic.get("礼金") != ""):
                            dic["礼金"] = int(float(dic["礼金"]))
                        if (dic.get("礼金")   != None and dic.get("礼金") != ""):
                            dic["礼金"] = int(float(dic["礼金"]))
                    count += 1

class MongoWriter:
    " ファイル名やdict型からmongoに書き込みを行うクラス. ファイルからの書き込みは FileReader と依存関係がある. "
    def __init__(self, db_name, collection_name):
        # self.keys = keys
        client = pymongo.MongoClient()
        self.db = client[db_name]
        self.collection = self.db[collection_name]


    def write_from_files(self, filenames):
        " ファイルからmongo に書き込みを行う. "
        file_reader = FileReader()
        for filename in filenames:
            file_reader.file_to_dict([filename])
            dicts = file_reader.dicts
            self.dict_to_mongo(dicts)
            print("written   " + filename)

    def dict_to_mongo(self, dicts):
        " dict 型からmongoに書き込みを行う. "
        for dic in dicts:
            try:
                dic['_id'] = ObjectId()
                posted = self.collection.insert_one(dic)
            except pymongo.errors.DuplicateKeyError as e:
                print(e)

def main():
    # dirname = "/Users/k.matsuura/Documents/mansion_db"
    dirname = "/Users/k.matsuura/Documents/mansion_db/2015/東京都/渋谷区"
    # dirname = "/Users/k.matsuura/Documents/mansion_db/2013"

    ### get csv filenames
    file_seeker = FileSeeker(ext='csv')
    file_seeker.seek_dir(dirname)
    filenames = file_seeker.filenames
    print("filename read end.")

    # mongo_writer = MongoWriter(db_name="db_all_2", collection_name="mansion")
    mongo_writer = MongoWriter(db_name="db_practice", collection_name="mansion_shibuya")

    mongo_writer.write_from_files(filenames)

if __name__ == '__main__':
    main()







