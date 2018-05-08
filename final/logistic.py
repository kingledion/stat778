#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Fri May  4 09:18:33 2018

@author: dhartig
"""

import numpy as np
import csv
from sklearn.linear_model import LogisticRegression




   

def getData(datatype):
    if datatype == 'stest':
        # test data
        X = np.array([[1, 1], [0, 1], [0, 0], [1, 1], [1, 0]])
        y = np.array([1,0,0,1,0])
        X = np.hstack((np.ones(len(y)).reshape(-1,1), X))
        
        return X, y
    
    if datatype == 'gausstest':
        n = 5000
        x1 = np.random.multivariate_normal([0, 0], [[1, .75],[.75, 1]], n)
        x2 = np.random.multivariate_normal([1, 4], [[1, .75],[.75, 1]], n)
        
        features = np.vstack((x1, x2))
        features = np.hstack((np.ones(n*2).reshape(-1,1), features))
        labels = np.hstack((np.zeros(n), np.ones(n)))
        
        return features, labels
    
    # pima data
    elif datatype == 'pima':
        with open("./pima.dat", 'r') as readin:
            rdr = csv.reader(readin, delimiter = ' ')
            next(rdr)
            
            arr = []
            resp = []
            for line in rdr:
                arr.append([float(i) for i in line[1:]])
                resp.append(float(line[0]))
        
        X = np.array(arr)
        X = (X - np.mean(X, axis=0))/np.std(X, axis=0) # standardize
        X = np.hstack((np.ones(X.shape[0]).reshape(-1,1), X)) # add intercept
        y = np.array(resp)
        return X, y
        
    elif datatype == 'pima_mod':
        with open("./pima.dat", 'r') as readin:
            rdr = csv.reader(readin, delimiter = ' ')
            next(rdr)
            
            arr = []
            for line in rdr:
                arr.append([float(i) for i in line[1:]])
        
        X = np.array(arr)
        X = (X - np.mean(X, axis=0))/np.std(X, axis=0) # standardize
        X = np.hstack((np.ones(X.shape[0]).reshape(-1,1), X)) # add intercept
        trueB = np.array([0,1,-1,1,0,0,0,0,0])
        y = np.rint(1/(1+np.exp(-1 * X @ trueB)))
        return X, y


def fitsklearn(X, y):
    model = LogisticRegression(fit_intercept = False, C=1e15)
    model.fit(X, y)
    
    print('sklearn.linear_model.LogisticRegression fit and .coef_')
    #print(model.predict(X))
    print(model.coef_)
    print()
    
def logistic(X, y):
    
    def lpi(X, B):
        return 1/(1+np.exp(-1*(X @ B)))
    
    def grad(X, y, lp):
        return  -1* X.T @ (y - lp)
    
    #def objective(X, y, lp):
    #    return -1*np.sum(y*np.log(lp) + (1-y) * np.log(1-lp))


    tol = 1e-6
    maxiter = 1000000
    gam = 1e-4
    
    delta = 1
    count = 0
    
    B = np.ones(len(X[0]))
    
    while (delta > tol and count < maxiter):
    
    
        lp = lpi(X, B)
        grd = grad(X, y, lp)
        Bnext = B - gam * grd
        delta = np.sqrt(np.sum((Bnext - B)**2))
        
        B = Bnext

        if (count % 100 == 0):
            print("B")
            print(" ".join("{0:.3f}".format(b) for b in B))
            print("Gradient")
            print(" ".join("{0:.3f}".format(g) for g in grd))
            print("Step: {0} complete; Last step dist: {1}\n".format(count, delta))

        count = count + 1
    
    print("Coefficients")
    print(B)
    print("Number of Iterations: {0}".format(count))
    
    
def logNR(X, y):
    
    def lpi(X, B):
        return 1/(1+np.exp(-1*(X @ B)))
    
    def gradient(X, y, lp):
        return  -1* X.T @ (y - lp)
    
    def hessian(X, y, lp):
        #print('D')
        #print(" ".join("{0:.3f}".format(i) for i in (lp * (1-lp))[:10]))
        D = np.diag(lp * (1 - lp))
        #print("lhs")
        lhs = X.T @ D
        #for i in range(lhs.shape[0]):
        #    print(" ".join("{0:.3f}".format(i) for i in lhs[i][:10]))
            
        #print("X")
        #for i in range(10):
        #    print(" ".join("{0:.3f}".format(i) for i in X[i]))
        
        #print(lhs.shape, X.shape)
        #print(np.dot(lhs[0], X[:,0]))
        #print("Hessian")
        #for j in range(lhs.shape[0]):
        #    print(" ".join("{0:.3f}".format(i) for i in (lhs @ X)[j]))
        return lhs @ X#X.T @ D @ X
    

    tol = 1e-6
    maxiter = 100

    delta = 1
    count = 0
    
    B = np.zeros(len(X[0]))
    
    while (delta > tol and count < maxiter):
        
        print("Current B")
        print(" ".join("{0:.3f}".format(i) for i in B))
    
    
        lp = lpi(X, B)
        #print("pi (first 10)")
        #print(" ".join("{0:.3f}".format(i) for i in lp[:10]))
        
        
        grad = gradient(X, y, lp)
        hess = hessian(X, y, lp)
        hinv = np.linalg.inv(hess)
        Bnext = B - hinv @ grad
        delta = np.sqrt(np.sum((Bnext - B)**2))
        
        print("Gradient")
        print(" ".join("{0:.3f}".format(i) for i in grad))

        print("Hessian inverse")
        for j in range(hinv.shape[0]):
            print(" ".join("{0:.3f}".format(i) for i in hinv[j]))
        #print(hinv)
        

        B = Bnext
        count = count + 1
        
        print("Step: {0} complete; Last step dist: {1}\n".format(count, delta))

    
    print("Coefficients")
    print(B)
    print("Number of Iterations: {0}".format(count))

    
    
def newlog(features, target):
    def sigmoid(scores):
        return 1 / (1 + np.exp(-scores))
    
    def log_likelihood(features, target, weights):
        scores = np.dot(features, weights)
        return np.sum(target*scores - np.log(1+np.exp(scores)))
    
    weights = np.zeros(features.shape[1])
    
    for step in range(300000):
        scores = np.dot(features, weights)
        predictions = sigmoid(scores)
        
        error = target - predictions
        gradient = np.dot(features.T, error)
        weights += 5e-5 * gradient
        
        if step % 10000 == 0:
            print(log_likelihood(features, target, weights))
            
    
    print("Coefficients")
    print(weights)
    
if __name__ == "__main__":
    
    X, y = getData('pima')
    print(X.shape, y.shape)
    
    fitsklearn(X, y)
    logNR(X, y)

    



