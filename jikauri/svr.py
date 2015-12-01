#!/usr/bin/env python
#  -*- coding:utf-8 -*-
#

import data as dat
import numpy as np
from sklearn import svm

class SVR:
    def predict(self, x, y, kernel='rbf', C=1e3, gamma='auto'):
        data = dat.Data()
        X = data.x_to_X(x)
        clf = svm.SVR(kernel=kernel, C=C, gamma=gamma)
        clf.fit(X, y)
        X, y_svr = clf.predict(X)
        return X, y_svr

