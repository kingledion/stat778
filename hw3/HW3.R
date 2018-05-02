library("survival")

data <- read.delim("/opt/school/stat778/hw3/HW2_2018.dat", header= FALSE, sep  = " ");
data <- data.frame(data)
colnames(data)<- c("Time", "Status", "cov1", "cov2");
res.cox <- coxph(Surv(Time, Status) ~ cov1 + cov2, data =  data)
summary(res.cox)

