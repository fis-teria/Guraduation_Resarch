num = "3"
switch(num,
"1" = setwd("D:\\openCV_WorkSpace\\Ohno_cmake_Desktop\\build\\result00LH"),
"2" = setwd("D:\\openCV_WorkSpace\\Ohno_cmake_Desktop\\build\\result01LH"),
"3" = setwd("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/Tsukuba02_locate"),
"4" = setwd("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/result01LH"),
stop("only use 1 2 3 4")
)
getwd()

x.data = c() #  空のベクトルを用意する．
y.data = c()
z.data = c()
u.data = c()
v.data = c()

data_size <- switch(num, #データの取得 if 00 -> 670  if 01 -> 488
"1" = 670, 
"2" = 488, 
"3" = 4546, 
"4" = 488,
stop("error") 
)

txt_name <- paste0("location.csv") #ここを自分の結果のファイルに合うように変えてね

txt_data <- read.table(txt_name, fileEncoding = "UTF-8", sep = ',')


x.data <- c(x.data, txt_data[[1]]) # ベクトルをつなげていく．
y.data <- c(y.data, txt_data[[2]])
z.data <- c(z.data, txt_data[[3]])

txt_name2 <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/r_file/location.csv") #ここを自分の結果のファイルに合うように変えてね

txt_data2 <- read.table(txt_name2, fileEncoding = "UTF-8", sep = ',')
u.data = c(u.data, txt_data2[[1]])
v.data = c(v.data, txt_data2[[2]])

for (i in 0:data_size) {
  #類似度
  if (i < 10) {
    txt_name <- paste0("location.csv") #ここを自分の結果のファイルに合うように変えてね
    png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/tsukuba_locate/00000", i, ".png")#ここを自分の結果のファイルに合うように変えてね
  } else if (i < 100) {
    txt_name <- paste0("location.csv") #ここを自分の結果のファイルに合うように変えてね
    png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/tsukuba_locate/0000", i, ".png") #ここを自分の結果のファイルに合うように変えてね
  } else if (i < 1000) {
    txt_name <- paste0("location.csv") #ここを自分の結果のファイルに合うように変えてね
    png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/tsukuba_locate/000", i, ".png") #ここを自分の結果のファイルに合うように変えてね
  } else if (i < 10000) {
    txt_name <- paste0("location.csv") #ここを自分の結果のファイルに合うように変えてね
    png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/tsukuba_locate/00", i, ".png") #ここを自分の結果のファイルに合うように変えてね
  }
  txt_data <- read.table(txt_name, fileEncoding = "UTF-8", sep = ',')
  
  start = 2 + i - 100
  if(start < 2){
    start = 2
  }
  end = 2 + i + 100
  if(end >data_size){
    end = data_size
  }

  #if (i == 1) {
  png(png_name, width = 1280, height = 360)
  split.screen(c(1,2)) # 1行2列  
  screen(1)
  par(mar = c(5, 5, 5, 10))
  plot(txt_data2[, 1][i + 1], txt_data2[, 2][i + 1], type = "p", xlim = range(x.data[start:end]), ylim = range(y.data[start:end]), col = 4, pch = c(4),cex = 4, ann = F) #  
  par(new = T)
  plot(txt_data[, 1][start:end], txt_data[, 2][start:end], type = "l", xlim = range(x.data[start:end]), ylim = range(y.data[start:end]), main = "Mini MAP", xlab = "Position X", ylab = "Position Y", col = 2) #  
  par(xpd = T)
  legend(par()$usr[2] + 0.6, par()$usr[4], legend = c("Now Location"), pch = c(4), col = c(4))

  screen(2)
  par(mar = c(5, 5, 5, 10))
  plot(txt_data[, 1], txt_data[, 2], type = "l", xlim = range(x.data), ylim = range(y.data), main = "ALL MAP", xlab = "Position X", ylab = "Position Y", col = 2) #  
  par(new = T)
  plot(txt_data2[, 1][i + 1], txt_data2[, 2][i + 1], type = "p", xlim = range(x.data), ylim = range(y.data), col = 4, pch = c(4),cex = 3, ann = F) #  
  par(xpd = T)
  legend(par()$usr[2] + 0.6, par()$usr[4], legend = c("Now Location"), pch = c(4), col = c(4))
  dev.off()

  #}  else {
  #points(txt_data[, 1], txt_data[, 2], type = "l", xlim = range(x.data), ylim = range(y.data)) #  
  #}
}