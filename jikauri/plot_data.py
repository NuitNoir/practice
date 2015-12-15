#!/usr/bin/env python
#  -*- coding:utf-8 -*-
#
# data plot class, 2d and 3d plot.
# depend data, regression modules
#

import pymongo
from matplotlib import cm
import matplotlib.pyplot as plt
from sklearn import svm, linear_model
import numpy as np
import data as dat
import math
import regression
import logging
from mpl_toolkits.mplot3d import Axes3D

def curve_fit_func(x: list, a, b0, b1, c):
    x = np.array(x)
    return a * np.exp(b0 * x[:,0] + b1 * x[:,1]) + c #+ b1*x + b2*x**2 + b3*x**3

class DataPlotter:
    " データプロットクラス "
    # def regress(self, x_datum: list, z_data: list, x_ranges: list, type='ridge') -> "X, y":
    #     reg = regression.Regression()
    #     func = curve_fit_func
    #     offset      = 1000
    #     data_num    = 1000
    #     # X, y = reg.predict(x_data[:data_num], x_data[data_num:data_num*2], y_data[:data_num], y_data[data_num:data_num*2], clf='polyfit', kernel='linear', func=func, C=30, gamma=0.3, degree=2) # clf='ridge')
    #
    #     X, y, regress_Z = reg.predict(x_datum[:offset+data_num], x_datum[offset+data_num:offset+data_num*2], z_data[:offset+data_num], z_data[offset+data_num:offset+data_num*2], x_ranges,
    #                                   clf_name='curvefit3d', kernel='rbf', func=func, C=C, gamma=gamma, degree=2) # clf='ridge')
    #
    #     return X, y, regress_Z
    #
    def plot_datum_3d(self, x_datum: list, z_data: list, Z: list, zlim: tuple, xlabels: str, dirname: str,
                      finds: str, x_axes: [str], z_axis: str, x_ranges: tuple,
                      type='surface', show=False, alpha=1, color=None, fig=None, ax=None) -> "fig, ax":
        " plot x_datum 3D. x_dastum ckolumn must be 2. "
        x_datum = np.array(x_datum)
        rows, cols = x_datum.shape
        if (cols < 2):
            raise Exception("x_datum columns must be equel or larger than 2.")
        xlim=(x_ranges[0][0], x_ranges[0][-1])
        ylim=(x_ranges[1][0], x_ranges[1][-1])

        if (type == 'surface'):
            fig, ax = self.plot_3d(x_ranges[0], x_ranges[1], z_data, Z, xlabels, xrange=xlim, yrange=ylim,
                         vmax = 10000, show=show, type=type)
        elif (type == 'scatter'):
            fig, ax = self.plot_3d(x_datum[:, 0], x_datum[:, 1], z_data, [], xlabels, xrange=xlim, yrange=ylim,
                                   vmax = 10000, show=show, type=type, alpha=alpha, color=color, fig=fig, ax=ax)
            # self.plot_3d(X1.flatten, X2.flatten, Z.flatten, [], xlabels, xrange=xlim, yrange=ylim,
            #              vmax = 10000, show=True, type=type, alpha=1)
        return fig, ax

    def get_means_3d(self, x_datum: "SampleSize-FeatureSize Matrix list", x_ranges: [tuple], z_data: "SampleSize list") -> [list]:
        """ 3次元データの z の平均値を matrix で返す.
         return Z = [ [] ]   """
        Z = np.ndarray((len(x_ranges[1]), len(x_ranges[0])))
        for x2 in x_ranges[1]:
            for x1 in x_ranges[0]:
                zs = []
                for i, x_val in enumerate(x_datum):
                    if (x1 <= x_val[0] < x1+1 and x2 <= x_val[1] < x2+1):
                        zs.append(z_data[i])
                mean = np.mean(np.array(zs))
                if (math.isnan(mean)):
                    # y_mean = y_means[-1] # 平均とれないときは直近のいれちゃう
                    mean = 0
                Z[x2][x1] = mean
        logging.debug("Z.shape = "+str(Z.shape))
        return Z


    def plot_3d(self, x: "N-length list", y: "M-length list", z: list, Z: "M-N matrix list", xlabels: [str], xrange=None, yrange=None, rstride=1, cstride=1,
                vmin=0, vmax=1, cmap=cm.RdYlGn, alpha=1, show=False, type='wire', color='None', fig=None, ax=None):
        " ３次元プロットを行う. Z は"
        if (fig == None):
            fig = plt.figure()
            ax = fig.add_subplot(111, projection='3d')
        ax.set_xlim(xrange) if (xrange) else "xrange none"
        ax.set_ylim(yrange) if (yrange) else "yrange none"
        ax.set_xlabel(xlabels[0])
        ax.set_ylabel(xlabels[1])
        ax.view_init(30, -140)
        if (type == 'wire'):
            X, Y = np.meshgrid(x, y)
            ax.plot_wireframe(X, Y, Z, rstride=rstride, cstride=cstride, color=color)
        elif (type == 'surface'):
            X, Y = np.meshgrid(x, y)
            ax.plot_surface(X, Y, Z, rstride=rstride, cstride=cstride,
                            cmap=cmap, vmin=vmin, vmax=vmax, alpha=alpha, color=color)
        elif (type == 'scatter'):
            ax.scatter(x, y, z, marker='.', alpha=alpha, color=color)
        if (show is True):
        	plt.show()
        return fig, ax

    def get_means(self, x_range, x_data, y_data):
        y_mean_pluses = []
        y_means = []
        y_mean_minuses = []
        # for x in range(x_range[0], x_range[1]):
        for x in x_range:
            ys = []
            for i, x_val in enumerate(x_data):
                if (x <= x_val < x+1):
                    ys.append(y_data[i])
            y_mean = np.mean(np.array(ys))
            y_std  = np.std(np.array(ys))
            y_mean_pluses.append(y_mean + y_std)
            if (math.isnan(y_mean)):
                # y_mean = y_means[-1] # 平均とれないときは直近のいれちゃう
                y_mean = 0
            y_means.append(y_mean)
            y_mean_minuses.append((y_mean - y_std))
        y_means = np.array(y_means)
        logging.debug("ymeans shape = "+str(y_means.shape))
        logging.debug("ymeans = "+str(y_means))
        return y_means

    def plot_datum(self, x_datum, y_data, ylim, xlabels, dirname, finds, x_axes, y_axis, x_ranges):
        " x_datum を それぞれプロットする. "
        plt.clf()
        x_datum = np.array(x_datum)
        rows, cols = x_datum.shape
        for col in range(0,cols):
            x_data = x_datum[:, col]
            plt.ylim(ylim)
            plt.xlabel(xlabels[col])
            plt.ylabel("rent per area [yen/m^2]")
            plt.scatter(x_data, y_data)

            y_means = self.get_means(x_ranges[col], x_data,  y_data)

            reg = regression.Regression()
            func = curve_fit_func
            offset    = 10000
            data_num    = 5000
            # X, y = reg.predict(x_data[:data_num], x_data[data_num:data_num*2], y_data[:data_num], y_data[data_num:data_num*2], clf='polyfit', kernel='linear', func=func, C=30, gamma=0.3, degree=2) # clf='ridge')
            X, y = reg.predict(x_data[:offset+data_num], x_data[offset+data_num:offset+data_num*2], y_data[:offset+data_num], y_data[offset+data_num:offset+data_num*2],
                               clf='ridge', kernel='rbf', func=func, C=1000, gamma=0.1, degree=2) # clf='ridge')

            plt.scatter(x_ranges[col], y_means, color='r')
            plt.scatter(X, y, color='y')
            filename = dirname+finds+'_'+x_axes[col]+'-'+y_axis+".png"
            plt.savefig(filename)

    def regularize_year(self, years):
        this_month = 20151101
        diffs = [year - this_month for year in years]
        ## 単位を年にする
        years = [diff/100000  for diff in diffs]
        return years

