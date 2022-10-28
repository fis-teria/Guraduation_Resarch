num = "2"
switch(num,
"1" = setwd("D:\\openCV_WorkSpace\\Ohno_cmake_Desktop\\build\\gra\\r_file"),
"2" = setwd("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/r_file"),
"3" = setwd("/mnt/d/openCV_WorkSpace/Ohno_cmake_Desktop/build/gra/r_file"),
stop("only use 1 2")
)
getwd()
x.data = c()
txt_name <- paste0("canny_histgram.csv") #ここを自分の結果のファイルに合うように変えてね
txt_data <- read.table(txt_name, fileEncoding = "UTF-8", sep = ',')
x.data = c(x.data, txt_data[[1]])
hist(x.data, breaks = seq(0,trunc(max(x.data)/100) * 100 + 100, 50))