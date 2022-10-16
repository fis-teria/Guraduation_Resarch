setwd("D:\\openCV_WorkSpace\\Ohno_cmake_Desktop\\build\\result00_CSV")
#setwd("D:\\openCV_WorkSpace\\Ohno_cmake_Desktop\\build\\result01_CSV")
#setwd("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/result00_CSV")
#setwd("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/result01_CSV")
getwd()

x.data = c() #  空のベクトルを用意する．
y.data = c()
z.data = c()
u.data = c()

data_size = 670 #データの取得 if 00 -> 670  if 01 -> 488

for (i in 1:data_size) {
  if (i < 10) {
    txt_name <- paste0("00000", i, ".csv") #ここを自分の結果のファイルに合うように変えてね
  } else if (i < 100) {
    txt_name <- paste0("0000", i, ".csv") #ここを自分の結果のファイルに合うように変えてね
  } else if (i < 1000) {
    txt_name <- paste0("000", i, ".csv") #ここを自分の結果のファイルに合うように変えてね
  }
  txt_data <- read.table(txt_name, fileEncoding = "UTF-8", sep = ',')


  x.data <- c(x.data, txt_data[[1]]) # ベクトルをつなげていく．
  y.data <- c(y.data, txt_data[[2]])
  z.data <- c(z.data, txt_data[[3]])
  u.data <- c(u.data, txt_data[[4]])
}


for (i in 1:data_size) {
  #類似度
  if (i < 10) {
    txt_name <- paste0("00000", i, ".csv") #ここを自分の結果のファイルに合うように変えてね
    png_name <- paste0("D:\\graResult\\result00_CSV\\00000", i, ".png") #ここを自分の結果のファイルに合うように変えてね
    #png_name <- paste0("D:\\graResult\\result01_CSV\\00000", i, ".png") #ここを自分の結果のファイルに合うように変えてね
    #png_name <- paste0("/home/user/Guraduation_Resarch/gra/result00_CSV/00000", i, ".png") #ここを自分の結果のファイルに合うように変えてね
    #png_name <- paste0("/home/user/Guraduation_Resarch/gra/result01_CSV/00000", i, ".png") #ここを自分の結果のファイルに合うように変えてね
  } else if (i < 100) {
    txt_name <- paste0("0000", i, ".csv") #ここを自分の結果のファイルに合うように変えてね
    png_name <- paste0("D:\\graResult\\result00_CSV\\0000", i, ".png") #ここを自分の結果のファイルに合うように変えてね
    #png_name <- paste0("D:\\graResult\\result01_CSV\\0000", i, ".png") #ここを自分の結果のファイルに合うように変えてね
    #png_name <- paste0("/home/user/Guraduation_Resarch/gra/result00_CSV/0000", i, ".png") #ここを自分の結果のファイルに合うように変えてね
    #png_name <- paste0("/home/user/Guraduation_Resarch/gra/result01_CSV/0000", i, ".png") #ここを自分の結果のファイルに合うように変えてね

  } else if (i < 1000) {
    txt_name <- paste0("000", i, ".csv") #ここを自分の結果のファイルに合うように変えてね
    png_name <- paste0("D:\\graResult\\result00_CSV\\000", i, ".png") #ここを自分の結果のファイルに合うように変えてね
    #png_name <- paste0("D:\\graResult\\result01_CSV\\000", i, ".png") #ここを自分の結果のファイルに合うように変えてね
    #png_name <- paste0("/home/user/Guraduation_Resarch/gra/result00_CSV/000", i, ".png") #ここを自分の結果のファイルに合うように変えてね
    #png_name <- paste0("/home/user/Guraduation_Resarch/gra/result01_CSV/000", i, ".png") #ここを自分の結果のファイルに合うように変えてね

  }
  txt_data <- read.table(txt_name, fileEncoding = "UTF-8", sep = ',')

  #if (i == 1) {
  png(png_name, width = 500, height = 300)
  par(mar = c(5, 5, 5, 8))
  plot(txt_data[, 1], txt_data[, 2], type = "l", ann = F, xlim = range(x.data), ylim = range(y.data), col = 2) #  
  par(new = T)
  plot(txt_data[, 1], txt_data[, 3], type = "l", ann = F, xlim = range(x.data), ylim = range(y.data), col = 4) #  
  par(new = T)
  plot(txt_data[, 1], txt_data[, 4], type = "l", xlim = range(x.data), ylim = range(y.data), main = txt_name, xlab = "被マッチング画像の番号", ylab = "類似度", col = 5) #  
  par(xpd = T)
  legend(par()$usr[2] + 0.2, par()$usr[4], legend = c("グレイスケール", "Canny", "Canny-small"), pch = c(1, 2, 3), col = c("red", "blue", "cyan"))
  par(xpd = T)
  legend(par()$usr[2] + 0.2, par()$usr[3], legend = c("最大値", which.max(txt_data[,2]), which.max(txt_data[,3]),which.max(txt_data[,4])), pch = c(0, 1, 2, 3), col = c(0, "red", "blue", "cyan"))
  dev.off()
  #}  else {
  #points(txt_data[, 1], txt_data[, 2], type = "l", xlim = range(x.data), ylim = range(y.data)) #  
  #}
}