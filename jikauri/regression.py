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
import logging
class Regression:
    def predict(self, train_x, test_x, train_y, test_y, x_ranges, clf_name='ridge', kernel='rbf', func=None, C=1, gamma='auto', degree=3):
        data = dat.Data()
        Z = []
        if (clf_name.count('3d')):
	        X = np.array(train_x)
	        test_X = np.array(test_x)
        else:
	        X       = data.x_to_X(train_x)
	        test_X  = data.x_to_X(test_x)
        y = []
        train_x = np.array(train_x)
        train_y = np.array(train_y)
        logging.debug("train_x shape = "+str(train_x.shape))
        logging.debug("train_y shape = "+str(train_y.shape))
        if (clf_name.count('fit')):
            if (clf_name == 'polyfit'):
                params = np.polyfit(train_x, train_y, 3)
                # print(params)
                logging.debug("polyfit parameter = "+str(params))
                p = np.poly1d(params)
                y = [p(x) for x in test_x]
            elif (clf_name == 'curvefit'):
                # popt, pcov = scipy.optimize.curve_fit(func, train_x, train_y, p0=[1,1,3000,1,1,1])
                popt, pcov = scipy.optimize.curve_fit(func, train_x, train_y, p0=[1,1,1,3000])
                logging.debug("curve_fit parameter = "+str(popt))
                # y = [func(x, popt[0], popt[1], popt[2], popt[3], popt[4], popt[5]) for x in test_x]
                y = [func(x, popt[0], popt[1], popt[2], popt[3]) for x in test_x]
            elif (clf_name == 'curvefit3d'):
                # popt, pcov = scipy.optimize.curve_fit(func, train_x, train_y, p0=[1,1,3000,1,1,1])
                popt, pcov = scipy.optimize.curve_fit(func, train_x, train_y, p0=[100,-1,-1,3000])
                logging.debug("curve_fit parameter = "+str(popt))
                # y = [func(x, popt[0], popt[1], popt[2], popt[3]) for x in test_x]
                for x2 in test_X[:,1]:
                    for x1 in test_X[:,0]:
                        y = func([[x1, x2]], popt[0], popt[1], popt[2], popt[3])
        else:
            if (clf_name.count('svr')):
                clf = svm.SVR(C=C, gamma=gamma, degree=degree)
                logging.debug('C = '+str(C)+' gamma = '+str(gamma)+' degree = '+str(degree ))
            elif (clf_name == 'ridgecv'):
                clf = linear_model.RidgeCV
            else: # (clf == 'ridge'):
                clf = linear_model.Ridge()
            clf.fit(X, train_y)
            # if (clf_name.count('3d')):
	         #    Z = clf.predict(x_ranges)
            y = clf.predict(test_X)
            logging.debug('regression y = '+str(y))
        try:
            rmse = math.sqrt(metrics.mean_squared_error(test_y, y))
            logging.debug(str(clf)+' RMSE ='+str(rmse))
        except Exception:
	        pass

        return test_X, y, Z





