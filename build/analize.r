num = "3"
switch(num,
"1" = setwd("D:\\openCV_WorkSpace\\Ohno_cmake_Desktop\\build\\result00_CSV"),
"2" = setwd("D:\\openCV_WorkSpace\\Ohno_cmake_Desktop\\build\\result01_CSV"),
"3" = setwd("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/result00_CSV"),
"4" = setwd("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/result00LH"),
stop("only use 1 2 3 4")
)
getwd()

x.data = c() #  空のベクトルを用意する．
y.data = c()
z.data = c()
u.data = c()

data_size <- switch(num, #データの取得 if 00 -> 670  if 01 -> 488
"1" = 670, 
"2" = 488, 
"3" = 670, 
"4" = 488,
stop("error") 
)

for (i in 0:data_size) {
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


for (i in 0:data_size) {
  #類似度
  if (i < 10) {
    txt_name <- paste0("00000", i, ".csv") #ここを自分の結果のファイルに合うように変えてね
    switch(num,
    "1" = png_name <- paste0("D:\\graResult\\result00_CSV\\00000", i, ".png"), #ここを自分の結果のファイルに合うように変えてね
    "2" = png_name <- paste0("D:\\graResult\\result01_CSV\\00000", i, ".png"), #ここを自分の結果のファイルに合うように変えてね
    "3" = png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/result00_CSV_temp/00000", i, ".png"), #ここを自分の結果のファイルに合うように変えてね
    "4" = png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/result00LH_LBP_small/00000", i, ".png"), #ここを自分の結果のファイルに合うように変えてね
    )
  } else if (i < 100) {
    txt_name <- paste0("0000", i, ".csv") #ここを自分の結果のファイルに合うように変えてね
    switch(num,
    "1" = png_name <- paste0("D:\\graResult\\result00_CSV\\0000", i, ".png"), #ここを自分の結果のファイルに合うように変えてね
    "2" = png_name <- paste0("D:\\graResult\\result01_CSV\\0000", i, ".png"), #ここを自分の結果のファイルに合うように変えてね
    "3" = png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/result00_CSV_temp/0000", i, ".png"), #ここを自分の結果のファイルに合うように変えてね
    "4" = png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/result00LH_LBP_small/0000", i, ".png"), #ここを自分の結果のファイルに合うように変えてね
    )
  } else if (i < 1000) {
    txt_name <- paste0("000", i, ".csv") #ここを自分の結果のファイルに合うように変えてね
    switch(num,
    "1" = png_name <- paste0("D:\\graResult\\result00_CSV\\000", i, ".png"), #ここを自分の結果のファイルに合うように変えてね
    "2" = png_name <- paste0("D:\\graResult\\result01_CSV\\000", i, ".png"), #ここを自分の結果のファイルに合うように変えてね
    "3" = png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/result00_CSV_temp/000", i, ".png"), #ここを自分の結果のファイルに合うように変えてね
    "4" = png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/result00LH_LBP_small/000", i, ".png"), #ここを自分の結果のファイルに合うように変えてね
    )

  }
  txt_data <- read.table(txt_name, fileEncoding = "UTF-8", sep = ',')
  png_name
  #if (i == 1) {
  png(png_name, width = 800, height = 800)
  par(mar = c(5, 5, 5, 9))
  plot(txt_data[, 1], txt_data[, 2], type = "l", xlim = range(x.data), ylim = range(y.data),main = txt_name, xlab = "被マッチング画像の番号", ylab = "類似度", col = 3, lwd = 2, cex.lab = 2) #  
  par(xpd = T)
  legend(par()$usr[2] + 0.5, par()$usr[4], legend = c("Small Temp","最大値", which.max(txt_data[,2])), pch = c(3, 3, 0), col = c("green", 3, 0), cex =1.3)
  dev.off()
  #}  else {
  #points(txt_data[, 1], txt_data[, 2], type = "l", xlim = range(x.data), ylim = range(y.data)) #  
  #}
}