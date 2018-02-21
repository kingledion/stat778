#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Feb 12 18:43:06 2018

@author: dhartig
"""

# Runs the hw1 executable in a subprocess, captures the output to stdout to get the data
# then plots a graph for part c of HW1


import subprocess, numpy as np
from matplotlib import pyplot as plt

times = []
values = []
variances = []

complete = subprocess.run("/opt/school/stat778/hw1/hw1", check=True, stdout=subprocess.PIPE)

for row in complete.stdout.decode("utf-8").splitlines():
    time, val, var = row.split(" ")
    times.append(float(time))
    values.append(float(val))
    variances.append(float(var))
    
vals = np.array(values)
    
# 95% confidence interval is 1.96*sqrt(variance)
variances = np.sqrt(variances) * 1.96
err_hi = vals**np.exp(np.divide(variances,(vals*np.log(vals))))
err_lo = vals**np.exp(np.divide(-1*variances,(vals*np.log(vals))))

plt.step(times, vals, 'b-', times, err_hi, 'r-', times, err_lo, 'r-')
plt.show()
    

