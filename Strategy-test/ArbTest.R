correlation <- function(y){
    y <- read.csv("ArbTest.csv")
    cl <- cor(y)
    cl
}

linearR <- function(y, x1, x2){
    lm(y[, x1] ~ y[, x2], data = y)
    lm(formula = y[, x1] ~ y[, x2], data = y)
}

adfTest <- function(y, x1, x2, v){
    nr <- nrow(y)
    sp <- numeric(nr)
    for(i in 1:nr){
        sp[i] <- y[i, x1] -v * y[i, x2]
    }
    adf.test(sp, k=0)
}
