#!/usr/bin/env python
#  -*- coding:utf-8 -*-
#
# ~/.../csv_to_mongo.py

import pymongo
import matplotlib.pyplot as plt
from sklearn import svm, linear_model
import numpy as np
import data as dat
import math
import regression
import svr

class DataPlotter:
    " データプロットクラス "
    def plot_datum(self, x_datum, y_data, ylim, xlabels, dirname, finds, x_axes, y_axis):
        " x_datum を それぞれプロットする. "
        for i, x_data in enumerate(x_datum):
            plt.ylim(ylim)
            plt.xlabel(xlabels[i])
            plt.ylabel("rent per area [yen/m^2]")
            plt.scatter(x_data, y_data)

            ####### 平均値
            y_mean_pluses = []
            y_means = []
            y_mean_minuses = []
            for x in range(0, 50):
                ys = []
                for i, x_val in enumerate(x_data):
                    if (x <= x_val < x+1):
                        ys.append(y_data[i])
                y_mean = np.mean(np.array(ys))
                y_std  = np.std(np.array(ys))
                y_mean_pluses.append(y_mean + y_std)
                if (math.isnan(y_mean)):
                    y_mean = y_means[-1] # 平均とれないときは直近のいれちゃう
                y_means.append(y_mean)
                y_mean_minuses.append((y_mean - y_std))

            # regression
            # data = dat.Data()
            # X = data.x_to_X(np.arange(0, 50))
            # clf = linear_model.Ridge()
            # clf.fit(X, y_means)
            # y = clf.predict(X)
            reg = regression.Regression()
            # X, y = reg.predict(np.arange(0,50), y_means, clf='svr', kernel='rbf', C=1e3, gamma=0.2) # clf='ridge')
            X, y = reg.predict(x_data[:5000], x_data[5000:10000], y_data[:5000], y_data[5000:10000], clf='polyfit', kernel='linear', C=30, gamma=0.3, degree=2) # clf='ridge')

            plt.scatter(range(0,50), y_means, color='r')
            plt.scatter(range(0,50), y_mean_pluses, color='g')
            plt.scatter(range(0,50), y_mean_minuses, color='g')

            plt.scatter(X, y, color='y')

            filename = dirname+finds+'_'+x_axes[0]+'-'+y_axis+".png"
            plt.savefig(filename)

    def regularize_year(self, years):
        this_month = 20151101
        diffs = [year - this_month for year in years]
        ## 単位を年にする
        years = [diff/100000  for diff in diffs]
        return years

def main():

    db_name, collection_name = 'db_1', 'mansion_shibuya_2015'
    client = pymongo.MongoClient()
    db = client[db_name]
    collection = db[collection_name]
    station = u'渋谷駅'
    # cursor = collection.aggregate([{'$match': {'駅名': station}}])
    cursor = collection.find({'駅名': station})
    # cursor.__repr__()

    x_axes = []
    x_axes.append(u"築年月")
    x_labels = ["building years [year]"]

    # x_axes.append(u"徒歩分数")
    # x_labels = ["walking time from station [min]"]

    # x_labels = ["walking time from station [min]", "building years [year]"]
    # x_axes.append(u"建物構造")
    x_datum = []
    y_axis = u"平米単価"
    y_data = []

    data = dat.Data()
    data_plotter = DataPlotter()
    data.get_datum_from_documents(x_axes, y_axis, cursor, x_datum, y_data)
    # data_plotter.regularize_year(x_datum[0, :])

    ## regression
    X = [[val] for val in x_datum[0]]
    X = np.array(X)
    print(X.shape)
    print(X)
    # svr_rbf = svm.SVR(kernel='rbf', C=1e3, gamma=0.1)
    # y_rbf   = svr_rbf.fit(X, y_data).predict(X)
    # svr_lin = svm.SVR(kernel='linear', C=1e3, gamma=0.1)
    # y_lin   = svr_lin.fit(X, y_data).predict(X)

    ylim = (0, 10000)
    # dirname = "~/Documents/graph/"
    dirname = "imgs/"
    finds = station
    data_plotter.plot_datum(x_datum, y_data, ylim, x_labels, dirname, finds, x_axes, y_axis)

if __name__ == '__main__':
    main()






