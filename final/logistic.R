dat <- read.csv("./pima.dat", header=TRUE, sep = ' ')

model <- glm(diabetes ~ x1+x2+x3+x4+x5+x6+x7+x8, data=dat, family=binomial)
summary(model)