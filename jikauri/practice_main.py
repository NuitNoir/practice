#!/usr/bin/env python
#  -*- coding:utf-8 -*-
#
# data plot and regress practice main module
#

import pymongo
from matplotlib import cm
import matplotlib.pyplot as plt
from sklearn import svm, linear_model
import numpy as np
import data as dat
import math
import regression
import svr
import logging
import plot_data
from mpl_toolkits.mplot3d import Axes3D

def curve_fit_func(x: list, a, b0, b1, c):
    x = np.array(x)
    return a * np.exp(b0 * x[:,0] + b1 * x[:,1]) + c #+ b1*x + b2*x**2 + b3*x**3

def regress(x_datum: list, z_data: list, x_ranges: list, type='ridge') -> "X, y":
    reg = regression.Regression()
    func = curve_fit_func
    offset      = 1000
    data_num    = 1000
    # X, y = reg.predict(x_data[:data_num], x_data[data_num:data_num*2], y_data[:data_num], y_data[data_num:data_num*2], clf='polyfit', kernel='linear', func=func, C=30, gamma=0.3, degree=2) # clf='ridge')
    X, y, regress_Z = reg.predict(x_datum[:offset+data_num], x_datum[offset+data_num:offset+data_num*2], z_data[:offset+data_num], z_data[offset+data_num:offset+data_num*2], x_ranges,
                       clf_name='curvefit3d', kernel='rbf', func=func, C=1000, gamma=0.1, degree=2) # clf='ridge')
    # X, y, regress_Z = reg.predict(x_datum[:offset+data_num], x_datum[offset+data_num:offset+data_num*2], z_data[:offset+data_num], z_data[offset+data_num:offset+data_num*2], x_ranges,
    #                    clf_name='svr', kernel='rbf', func=func, C=1000, gamma=0.1, degree=2) # clf='ridge')

    return X, y, regress_Z

def main():
    FORMAT = '%(asctime)-15s %(clientip)s %(user)-8s %(message)s'
    logging.basicConfig(filename='log/practice_main.log',level=logging.DEBUG)
    logging.basicConfig(format=FORMAT)
    db_name, collection_name = 'db_1', 'mansion_shibuya_2015'
    client = pymongo.MongoClient()
    db = client[db_name]
    collection = db[collection_name]
    station = u'渋谷駅'
    # cursor = collection.aggregate([
    #     {'$match': {'駅名': station}},
    #     {'$group': {'徒歩分数': '$徒歩分数'}}
    # ])

    cursor = collection.find({'駅名': station})

    x_axes = [u"徒歩分数", u"築年月"]
    x_ranges = [range(0, 20), range(0, 50)]
    x_labels = ["walking time from station [min]", "building years [year]"]

    x_datum = []
    y_axis = u"平米単価"
    y_data = []

    data = dat.Data()
    data_plotter = plot_data.DataPlotter()
    data.get_datum_from_documents(x_axes, y_axis, cursor, x_datum, y_data)

    ylim = (0, 10000)
    dirname = "imgs/"
    finds = station

    np.savetxt("data/shibuya_2015.txt", x_datum)

    X, regress_z, regress_Z = regress(x_datum, y_data, x_ranges)

    # data_plotter.plot_datum(x_datum, y_data, regress_Z, ylim, x_labels, dirname, finds, x_axes, y_axis, x_ranges)
    # fig, ax = data_plotter.plot_datum_3d(x_datum, y_data, ylim, x_labels, dirname, finds,
    #                                      x_axes, y_axis, x_ranges, type='surface', show=True)
    # filename = dirname+"3D/"+finds+'_'+x_axes[0]+'&'+x_axes[1]+'-'+y_axis+"_mean.png"
    # fig.savefig(filename)

    ## plot regressed data points
    fig, ax = data_plotter.plot_datum_3d(X, regress_z, regress_Z, ylim, x_labels, dirname, finds, x_axes, y_axis, x_ranges,
                                         type='scatter', color='r', alpha=1)
    ## plot data points
    fig, ax = data_plotter.plot_datum_3d(x_datum, y_data, regress_Z, ylim, x_labels, dirname, finds, x_axes, y_axis, x_ranges,
                                         type='scatter', fig=fig, ax=ax, show=True, alpha=0.1)
    filename = dirname+"3D/"+finds+'_'+x_axes[0]+'&'+x_axes[1]+'-'+y_axis+"_scatter.png"
    fig.savefig(filename)

if __name__ == '__main__':
    main()
