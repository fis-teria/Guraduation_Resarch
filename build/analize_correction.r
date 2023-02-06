num = "2"
switch(num,
"1" = setwd("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/result00_CSV"),
"2" = setwd("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/result00LH"),
stop("only use 1 2")
)
getwd()

x.data = c() #  空のベクトルを用意する．
y.data = c()
z.data = c()
u.data = c()
v.data = c()
peak = c()

data_size <- 670 #データの取得 if 00 -> 670  if 01 -> 488

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
  #v.data <- c(v.data, txt_data[[5]])
}

rows = 4
for (i in 0:data_size) {
  #類似度
  if (i < 10) {
    txt_name <- paste0("00000", i, ".csv") #ここを自分の結果のファイルに合うように変えてね
  } else if (i < 100) {
    txt_name <- paste0("0000", i, ".csv") #ここを自分の結果のファイルに合うように変えてね
  } else if (i < 1000) {
    txt_name <- paste0("000", i, ".csv") #ここを自分の結果のファイルに合うように変えてね
    switch(num,
    )

  }
  txt_data <- read.table(txt_name, fileEncoding = "UTF-8", sep = ',')
　peak <- c(peak, sort(txt_data[, rows],decreasing=TRUE)[1]/(sort(txt_data[, rows],decreasing=TRUE)[1] + sort(txt_data[, rows],decreasing=TRUE)[2]))
}

#png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/peak/peak_temp.png")
#png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/peak/peak_Canny.png")
#png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/peak/peak_Canny_small.png")
#png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/peak/peak_LBP.png")
#png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/peak/peak_LBP_small.png")
png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/peak/peak_HOG.png")
#png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/peak/peak_HOG_small.png")
png(png_name, width = 800, height = 800)
par(mar = c(5, 5, 5, 15))
plot(0:670, peak, type = "l", xlim = range(0:670), ylim = range(4:10)*0.1, main = "精度の検証", xlab = "被マッチング画像の番号", ylab = "ピークと2番目に大きい値の比", col = 3, lwd = 2, cex.lab = 2) #  
par(xpd = T)
legend(par()$usr[2] + 0.6, par()$usr[4], legend = c("HOG", "最大値" , format(peak[which.max(peak)], nsmall = 5),"最小値", format(peak[which.min(peak)], nsmall = 5),"平均値",format(mean(peak), nsmall = 5), "最大の位置", which.max(peak)), pch = c(1, 1, 0,1,0,1,0,1,0), col = c(3, 3, 0,3,0,3,0,3,0),cex = 1.5)
dev.off()
#    "1" = png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/result00_CSV"), #ここを自分の結果のファイルに合うように変えてね
#    "2" = png_name <- paste0("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/result00LH"), #ここを自分の結果のファイルに合うように変えてね
