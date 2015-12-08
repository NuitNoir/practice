# -*- coding:utf-8 -*-
import pymongo
import numpy as np
import logging

class Data:
    " データ取得や変換を行うクラス "
    def x_to_X(self, x):
        " １次元配列の x=[] を2次元配列の X=[ [] ](行列形式) に変換する. "
        X = [ [float(val)] for val in x]
        X = np.array(X, dtype=float)
        return X

    def get_datum_from_documents(self, x_axes, y_axis, documents, x_datum, y_data):
        " x軸のデータ郡を複数取得. x_datum = [ [] ] "
        # for i, x_axis in enumerate(x_axes):
        #     logging.debug("x_axis "+str(i)+" = "+str(x_axis))
        #     x_datum.append([])
            # _documents = documents.alive()
        logging.debug("x_axes = "+str(x_axes))
        for document in documents:
                try:
                    yval = float(document[y_axis])
                    x_data = []
                    for i, x_axis in enumerate(x_axes):
                        # x_datum.append([])
                        xval = float(document[x_axis])
                        ### FIXME ループの中で判断させない
                        if (x_axis == u"築年月"):
                            # TODO 現在の年月(01日)を取得させる
                            xval = (20151201 - xval) / 10000
                        x_data.append(xval)
                    # if (self.is_distinct(x_datum, x_data, y_data, yval)):
                    y_data.append(yval)
                    x_datum.append(x_data)
                    # x_datum[i].append(xval)
                except KeyError:
                    logging.error("Key Error Occured")
                except ValueError:
                    logging.error(document[x_axis])
                    logging.error(document[y_axis])
                    logging.error("Value Error Occured at "+x_axis+" or "+y_axis)
        x_datum = np.array(x_datum)
        y_data  = np.array(y_data)
        logging.debug("x_datum shape = " + str(x_datum.shape))
        logging.debug("x_datum = " +str(x_datum))
        logging.debug("y_data shape = " + str(y_data.shape))
        logging.debug("y_data = "+str(y_data))

    def is_distinct(self, x_datum, x_data, y_data, yval):
        for i, x in enumerate(x_datum):
            distinct_flag = False
            ## i番目のデータといずれかの要素で異なればdistinctなデータ
            for col, xval in enumerate(x):
                if (xval != x_data):
                    distinct_flag = True
            if (y_data[i] != yval):
                distinct_flag = True
            if (distinct_flag == False):
                return False
        else:
            return True


