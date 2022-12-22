setwd("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/r_file")
getwd()

x.data = c() #  空のベクトルを用意する．
y.data = c()
z.data = c()

u.data = c()
v.data = c()
w.data = c()

i.data = c()
j.data = c()
k.data = c()

data_size = 4546

txt_name <- paste0("location_LBP.csv") #ここを自分の結果のファイルに合うように変えてね

txt_data <- read.table(txt_name, fileEncoding = "UTF-8", sep = ',')

x.data <- c(x.data, txt_data[[1]]) # ベクトルをつなげていく．
y.data <- c(y.data, txt_data[[2]])
z.data <- c(z.data, txt_data[[3]])

txt_name2 <- paste0("location_HOG.csv") #ここを自分の結果のファイルに合うように変えてね

txt_data2 <- read.table(txt_name2, fileEncoding = "UTF-8", sep = ',')
u.data = c(u.data, txt_data2[[1]])
v.data = c(v.data, txt_data2[[2]])
v.data = c(w.data, txt_data2[[3]])

txt_name3 <- paste0("location_Canny.csv") #ここを自分の結果のファイルに合うように変えてね

txt_data3 <- read.table(txt_name3, fileEncoding = "UTF-8", sep = ',')
i.data = c(i.data, txt_data3[[1]])
j.data = c(j.data, txt_data3[[2]])
k.data = c(k.data, txt_data3[[3]])

png_name <- paste0("location_LBP.png")#ここを自分の結果のファイルに合うように変えてね
#if (i == 1) {
png(png_name, width = 1280, height = 360)
par(mar = c(5, 5, 5, 10))
plot(range(0:4546), txt_data[, 3], type = "p", xlim = range(0:4546), ylim = range(0:4821), xlab = "取得したフレーム番号", ylab = "マッチしたフレーム番号",col = 4, pch = c(4),cex = 4, ann = F) #  
par(xpd = T)
legend(par()$usr[2] + 0.6, par()$usr[4], legend = c("Location Num"), pch = c(4), col = c(4))
dev.off()

png_name <- paste0("location_HOG.png")#ここを自分の結果のファイルに合うように変えてね
#if (i == 1) {
png(png_name, width = 1280, height = 360)
par(mar = c(5, 5, 5, 10))
plot(range(0:4546), txt_data2[, 3], type = "p", xlim = range(0:4546), ylim = range(0:4821), xlab = "取得したフレーム番号", ylab = "マッチしたフレーム番号",col = 4, pch = c(4),cex = 4, ann = F) #  
par(xpd = T)
legend(par()$usr[2] + 0.6, par()$usr[4], legend = c("Location Num"), pch = c(4), col = c(4))
dev.off()

png_name <- paste0("location_Canny.png")#ここを自分の結果のファイルに合うように変えてね
#if (i == 1) {
png(png_name, width = 1280, height = 360)
par(mar = c(5, 5, 5, 10))
plot(range(0:4546), txt_data3[, 3], type = "p", xlim = range(0:4546), ylim = range(0:4821), xlab = "取得したフレーム番号", ylab = "マッチしたフレーム番号",col = 4, pch = c(4),cex = 4, ann = F) #  
par(xpd = T)
legend(par()$usr[2] + 0.6, par()$usr[4], legend = c("Location Num"), pch = c(4), col = c(4))
dev.off()
