# -*- coding:utf-8 -*-
import pymongo
import numpy as np

class Data:
    " データ取得や変換を行うクラス "
    def x_to_X(self, x):
        " １次元配列の x=[] を2次元配列の X=[ [] ](行列形式) に変換する. "
        X = [ [float(val)] for val in x]
        X = np.array(X, dtype=float)
        return X

    def get_datum_from_documents(self, x_axes, y_axis, documents, x_datum, y_data):
        " x軸のデータ郡を複数取得. x_datum = [ [] ] "
        for i, x_axis in enumerate(x_axes):
            print(i)
            print(x_axis)
            x_datum.append([])
            # _documents = documents.alive()
            for document in documents:
                try:
                    xval, yval = float(document[x_axis]), float(document[y_axis])
                    ### FIXME ループの中で判断させない
                    if (x_axis == u"築年月"):
                        xval = (20151201 - xval) / 10000
                    x_datum[i].append(xval)
                    y_data.append(yval)
                except KeyError as e:
                    print "Key Error Occured at " + x_axis +" or "+y_axis
                except ValueError as v:
                    print document[x_axis]
                    print document[y_axis]
                    print "Value Error Occured at "+x_axis+" or "+y_axis
