#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Feb 12 18:43:06 2018

@author: dhartig
"""

import subprocess
from matplotlib import pyplot as plt
from math import sqrt

times = []
values = []
variances = []

complete = subprocess.run("/opt/school/stat778/hw1/hw1", check=True, stdout=subprocess.PIPE)

for row in complete.stdout.decode("utf-8").splitlines():
    time, val, var = row.split(" ")
    times.append(float(time))
    values.append(float(val))
    variances.append(float(var))
    
# 95% confidence interval is 1.96*sqrt(variance)
variances = [1.96*sqrt(v) for v in variances]

plt.errorbar(times, values, yerr=variances, fmt = 'bo', ecolor='r', capsize=2)
plt.show()
    

