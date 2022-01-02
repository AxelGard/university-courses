library(glue)
library(gtools)
library(ggplot2)
printf <- defmacro(str, expr = {print(glue(str))})

# import data and scale it 
data = read.csv("communities.csv")
response = data$ViolentCrimesPerPop
features = as.data.frame(scale(data))
features$ViolentCrimesPerPop = c()

