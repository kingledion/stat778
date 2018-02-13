#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Mon Feb 12 17:02:54 2018

@author: dhartig
"""

from lifelines import KaplanMeierFitter
import csv

times = []
events = []

with open("/opt/school/stat778/hw1/HW1.dat") as csvin: 
    rdr = csv.reader(csvin, delimiter=' ')
    for time, flag in rdr:
        times.append(float(time))
        events.append(int(flag))

kmf = KaplanMeierFitter()
kmf.fit(times, event_observed = events)

print(kmf.survival_function_)