#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri May  4 11:10:23 2018

@author: dhartig
"""

import numpy as np



def objective(X, y, lpi):
    return np.sum(np.log(lpi))


lpi = np.random.rand(10)
print(objective(lpi, lpi, lpi))