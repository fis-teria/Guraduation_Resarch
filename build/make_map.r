num = "2"
switch(num,
"1" = setwd("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/Tsukuba00_locate"),
"2" = setwd("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/Tsukuba01_locate"),
"3" = setwd("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/Tsukuba02_locate"),
stop("only use 1 2 3")
)
getwd()

x.data = c() #  空のベクトルを用意する．
y.data = c()
z.data = c()
u.data = c()
v.data = c()

data_size <- switch(num, #データの取得 if 00 -> 670  if 01 -> 488
"1" = 4546, 
"2" = 2469, 
"3" = 4548, 
stop("error") 
)

  txt_name <- paste0("location.csv") #ここを自分の結果のファイルに合うように変えてね
  txt_data <- read.table(txt_name, fileEncoding = "UTF-8", sep = ',')


  x.data <- c(x.data, txt_data[[1]]) # ベクトルをつなげていく．
  y.data <- c(y.data, txt_data[[2]])
  z.data <- c(z.data, txt_data[[3]])
  u.data <- c(u.data, txt_data[[4]])
  v.data <- c(v.data, txt_data[[5]])


    txt_name <- paste0("location.csv") #ここを自分の結果のファイルに合うように変えてね
    switch(num,
    "1" = png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/Tsukuba00_locate/locate.png"), #ここを自分の結果のファイルに合うように変えてね
    "2" = png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/Tsukuba01_locate/locate.png"), #ここを自分の結果のファイルに合うように変えてね
    "3" = png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/Tsukuba02_locate/locate.png"), #ここを自分の結果のファイルに合うように変えてね
    )
  txt_data <- read.table(txt_name, fileEncoding = "UTF-8", sep = ',')

  #if (i == 1) {
  png(png_name, width = 600, height = 400)
  par(mar = c(5, 5, 5, 8))
  plot(txt_data[, 1], txt_data[, 2], type = "l", xlim = range(x.data), ylim = range(y.data), main = txt_name, xlab = "被マッチング画像の番号", ylab = "類似度", col = 3) #  
  par(xpd = T)
  legend(par()$usr[2] + 0.6, par()$usr[4], legend = c("HOG", "最大値", which.max(txt_data[,2])), pch = c(1, 0, 1), col = c(3, 0, 0))
  dev.off()
