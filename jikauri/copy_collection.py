# -*- coding:utf-8 -*-
import pymongo

db_name = "db_1"
client = pymongo.MongoClient()
db = client[db_name]
in_collection = db["mansion_shibuya_2015_4"]
out_collection = db["mansion_shibuya_2015_4_index"]

cursor = in_collection.find({})
print(cursor.count())

for doc in cursor:
    try:
        out_collection.insert(doc)
    except:
        # print(doc)
        pass
