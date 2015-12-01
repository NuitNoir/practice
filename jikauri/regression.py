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
    def predict(self, train_x, test_x, train_y, test_y, clf='ridge', kernel='rbf', func=None, C=1, gamma='auto', degree=3):
        data = dat.Data()
        X = data.x_to_X(train_x)
        test_X = data.x_to_X(test_x)
        y = []
        if (clf == 'polyfit' or clf == 'curvefit'):
            if (clf == 'polyfit'):
                params = np.polyfit(train_x, train_y, 3)
                print(params)
                p = np.poly1d(params)
                y = [p(x) for x in test_x]
            elif (clf == 'curvefit'):
                # popt, pcov = scipy.optimize.curve_fit(func, train_x, train_y, p0=[1,1,3000,1,1,1])
                popt, pcov = scipy.optimize.curve_fit(func, train_x, train_y, p0=[1,1,3000])
                print(popt)
                # y = [func(x, popt[0], popt[1], popt[2], popt[3], popt[4], popt[5]) for x in test_x]
                y = [func(x, popt[0], popt[1], popt[2]) for x in test_x]
        else:
            if (clf == 'svr'):
                clf = svm.SVR(C=C, gamma=gamma, degree=degree)
            elif (clf == 'ridgecv'):
                clf = linear_model.RidgeCV
            else: # (clf == 'ridge'):
                clf = linear_model.Ridge()
            clf.fit(X, train_y)
            y = clf.predict(test_X)
            print(y)
            print(y.shape)
        rmse = math.sqrt(metrics.mean_squared_error(test_y, y))
        print(rmse)

        return test_X, y





