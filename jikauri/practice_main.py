#!/usr/bin/env python
#  -*- coding:utf-8 -*-
#
# data plot and regress practice main module
#

import pymongo
from matplotlib import cm
import matplotlib.pyplot as plt
from sklearn import svm, linear_model
import sklearn
import numpy as np
import data as dat
import math
import regression
import svr
import logging
import plot_data
from mpl_toolkits.mplot3d import Axes3D

def curve_fit_func(x, a, b00, b01, b10, b11, c):
    x = np.array(x)
    return a * np.exp(b00*x[:,0] + b01*x[:,0]**2 + b10*x[:,1] + b11*x[:,1**2]) + c #+ b1*x + b2*x**2 + b3*x**3

def poly_func(x, a0, a1, a2, b1, b2, b3):
    x = np.array(x)
    # print(np.mul(x[:,0], x[:,0]))
    return a0 + a1*x[:, 0] + a2*x[:, 0]*2 + b1*x[:, 1] + b2*x[:, 1]*2 + b3*x[:, 1]**3

def regress(x_datum: list, z_data: list, x_ranges: list) -> "X, y":
    x_datum = np.array(x_datum)
    z_data = np.array(z_data)
    reg = regression.Regression()
    func = curve_fit_func

    kf = sklearn.cross_validation.KFold(n=z_data.__len__(), n_folds=5, shuffle=False)
    C     = 1
    gamma = 0.1

    Cs = [1, 10, 1e2, 1e3, 1e4, 1e5, 1e6, 1e7, 1e8, 1e9]
    Cs = [1e4, 1e5, 1e6, 1e7]
    gammas = [1e-2, 1e-3, 1e-4, 1e-5, 1e-6]
    for C in Cs:
        for gamma in gammas:
            rmses = []
            for train_index, test_index in kf:
                X_train, X_test = x_datum[train_index], x_datum[test_index]
                y_train, y_test = z_data[train_index], z_data[test_index]
                X, y, regress_Z, rmse, clf, params = reg.predict(X_train, X_test, y_train, y_test, x_ranges,
                                                clf_name='svr', kernel='rbf', func=func, C=C, gamma=gamma, degree=3)
                # print("params = "+str(params)+"\tRMSE = "+str(rmse))
                rmses.append(rmse)
            rmses = np.array(rmses)
            # print("mean_rmse = "+str(rmses.mean())+"\tstd_rmse = "+str(rmses.std()))
            print("C = "+str(C)+"\tgamma = "+str(gamma)+"\tmean_rmse = "+str(rmses.mean())+"\tstd_rmse = "+str(rmses.std()))

    C = 1e6
    gamma = 1e-3
    for train_index, test_index in kf:
        # print("TRAIN:", train_index, "TEST:", test_index)
        X_train, X_test = x_datum[train_index], x_datum[test_index]
        y_train, y_test = z_data[train_index], z_data[test_index]
        X, y, regress_Z, rmse, clf, params = reg.predict(X_train, X_test, y_train, y_test, x_ranges,
                                        clf_name='svr3d', kernel='rbf', func=func, C=C, gamma=gamma, degree=2)
        print("C = "+str(C)+" gamma = "+str(gamma)+" rmse"+str(rmse))


    # X, y, regress_Z = reg.predict(x_datum[:offset+data_num], x_datum[offset+data_num:offset+data_num*2], z_data[:offset+data_num], z_data[offset+data_num:offset+data_num*2], x_ranges,
    #                    clf_name='svr', kernel='rbf', func=func, C=1000, gamma=0.1, degree=2) # clf='ridge')

    return X, y, regress_Z, rmse, clf

def main():
    FORMAT = '%(asctime)-15s %(clientip)s %(user)-8s %(message)s'
    logging.basicConfig(filename='log/practice_main.log',level=logging.DEBUG)
    logging.basicConfig(format=FORMAT)
    db_name, collection_name = 'db_1', 'mansion_shibuya_2015_4_index'
    client = pymongo.MongoClient()
    db = client[db_name]
    collection = db[collection_name]
    station = u'渋谷駅'
    # cursor = collection.aggregate([
    #     {'$match': {'駅名': station}},
    #     {'$group': {'徒歩分数': '$徒歩分数'}}
    # ])
    cursor = collection.find({'駅名': station})
    print(cursor.count())

    x_axes = [u"専有面積", u"築年月"]
    x_ranges = [range(0, 100), range(0, 50)]
    # x_labels = ["walking time from station [min]", "building years [year]"]
    x_labels = ["area [m^2]", "building years [year]"]

    x_datum = []
    y_axis = u"平米単価"
    y_data = []

    data = dat.Data()
    data_plotter = plot_data.DataPlotter()
    data.get_datum_from_documents(x_axes, y_axis, cursor, x_datum, y_data)

    ylim = (0, 10000)
    dirname = "imgs/"
    finds = station

    # x_datum = np.array(x_datum)
    # y_data = np.array(y_data)
    # print("x_datum.shape = "+str(x_datum.shape)+" y_data.shape = "+str(y_data.shape))
    np.savetxt("data/shibuya_2015.txt", x_datum)

    # data_plotter.plot_datum(x_datum, y_data, regress_Z, ylim, x_labels, dirname, finds, x_axes, y_axis, x_ranges)
    # fig, ax = data_plotter.plot_datum_3d(x_datum, y_data, ylim, x_labels, dirname, finds,
    #                                      x_axes, y_axis, x_ranges, type='surface', show=True)
    # filename = dirname+"3D/"+finds+'_'+x_axes[0]+'&'+x_axes[1]+'-'+y_axis+"_mean.png"
    # fig.savefig(filename)
    reg = regression.Regression()

    X, regress_z, regress_Z, rmse, clf = regress(x_datum, y_data, x_ranges)
    C     = 1e6
    gamma = 1e-5
    X, regress_z, regress_Z, rmse, clf, params = reg.predict(x_datum, x_datum, y_data, y_data, x_ranges,
                                                    clf_name='svr3d', kernel='rbf', C=C, gamma=gamma, degree=2)
    fig, ax = data_plotter.plot_datum_3d(X, regress_z, regress_Z, ylim, x_labels, dirname, finds, x_axes, y_axis, x_ranges,
                                         type='scatter', color='r', alpha=1)
    ## plot data points
    fig, ax = data_plotter.plot_datum_3d(x_datum, y_data, regress_Z, ylim, x_labels, dirname, finds, x_axes, y_axis, x_ranges,
                                         type='scatter', fig=fig, ax=ax, alpha=0.1)
    filename = dirname+"3D/"+finds+'_'+x_axes[0]+'&'+x_axes[1]+'-'+y_axis+"_scatter.png"
    fig.savefig(filename)
    # plt.show()
    ### 3Dプロット用のZ
    # Z = np.array((x_ranges[1][-1], x_ranges[0][-1]))
    # for i, y in enumerate(x_ranges[1]):
    #     for j, x in enumerate(x_ranges[0]):
    #         Z[i][j] = clf.predict([].append([x,y]))
    # fig, ax = data_plotter.plot_datum_3d(x_ranges[0], x_ranges[1], Z, ylim, x_labels, dirname, finds, x_axes, y_axis, x_ranges,
    #                                      type='surface', fig=fig, ax=ax, alpha=0.1)


    # X, regress_z, regress_Z, rmse, clf, params = reg.predict(X, X, regress_z, regress_z, x_ranges,
    #            clf_name='polyfit', func=curve_fit_func, degree=6)
    # X, regress_z, regress_Z, rmse, clf, params = reg.predict(x_datum, x_datum, y_data, y_data, x_ranges,
    #            clf_name='curvefit', func=poly_func, degree=6)
    ## xplot curve fitted figure
    fig, ax = data_plotter.plot_datum_3d(X, regress_z, regress_Z, ylim, x_labels, dirname, finds, x_axes, y_axis, x_ranges,
                                         type='scatter', color='g', alpha=1)
    filename = dirname+"3D/"+finds+'_'+x_axes[0]+'&'+x_axes[1]+'-'+y_axis+"_scatter.png"
    fig.savefig(filename)
    plt.show()

if __name__ == '__main__':
    main()
