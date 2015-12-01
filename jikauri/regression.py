#!/usr/bin/env python
#  -*- coding:utf-8 -*-
#
# 回帰を行う
#

import pymongo
import plot_data
import data as dat
from sklearn import linear_model, svm, metrics
import numpy as np
import matplotlib.pyplot as plt
import math
import scipy

class Regression:
    def predict(self, train_x, test_x, train_y, test_y, clf='ridge', kernel='rbf', C=1, gamma='auto', degree=3):
        data = dat.Data()
        X = data.x_to_X(train_x)
        test_X = data.x_to_X(test_x)
        y = []
        if (clf == 'polyfit'):
            params = np.polyfit(train_x, train_y, 3)
            p = np.poly1d(params)
            y = [p(x) for x in test_x]
            # test_X = X
        else:
            if (clf == 'svr'):
                clf = svm.SVR(C=C, gamma=gamma, degree=degree)
            elif (clf == 'ridgecv'):
                clf = linear_model.RidgeCV
            else: # (clf == 'ridge'):
                clf = linear_model.Ridge()

            clf.fit(X, train_y)
            y = clf.predict(test_X)
            rmse = math.sqrt(metrics.mean_squared_error(test_y, y))
            print(rmse)

        return test_X, y



def main():
    db_name, collection_name = 'db_1', 'mansion_shibuya_2015'
    client = pymongo.MongoClient()
    db = client[db_name]
    collection = db[collection_name]
    print collection.count()
    # cursor = collection.aggregate([{'$match': {"所在地1": "千葉市中央区"}}])
    station = '原宿駅'
    cursor = collection.aggregate([{'$match': {'駅名': station}}])

    x_axes = []
    x_axes.append(u"徒歩分数")
    x_axes.append(u"築年月")
    # x_axes.append(u"建物構造")
    x_datum = []
    y_axis = u"平米単価"
    ydata = []

    data_plotter = plot_data.DataPlotter()
    data_plotter.get_datum_from_documents(x_axes, y_axis, cursor, x_datum, ydata)
    ylim = (0, 10000)
    xlabels = ["walking time from station [min]", "building years [year]"]






