#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Feb 12 17:02:54 2018

@author: dhartig
"""

from lifelines import CoxPHFitter
import csv, pandas as pd, numpy as np

times = []
events = []
x1 = []
x2 = []

with open("/opt/school/stat778/hw2/HW2_2018.dat") as csvin: 
    rdr = csv.reader(csvin, delimiter=' ')
    for time, flag, cov1, cov2 in rdr:
        times.append(float(time))
        events.append(int(flag))
        x1.append(float(cov1))
        x2.append(int(cov2))
        
df = pd.DataFrame({'time': times, 'censored': events, 'x1': x1, 'x2': x2})

cph = CoxPHFitter()
cph.fit(df, duration_col='time', event_col='censored')
cph.print_summary()
h = cph.predict_log_hazard_relative_to_mean(np.array([[0.5], [0.5]]))
print(cph._log_likelihood)
print(h)


#kmf = KaplanMeierFitter()
#kmf.fit(times, event_observed = events)

#print(kmf.survival_function_)