setwd("/home/user/Guraduation_Resarch/Guraduation_Resarch/build/gra/slide/TempImg_divide/hist")
getwd()

data_size = 702
for (i in 0:data_size) {
  #類似度
  if (i < 10) {
    txt_name <- paste0("hist000", i, ".csv") #ここを自分の結果のファイルに合うように変えてね
    png_name <- paste0("_hist000",i,".png")#ここを自分の結果のファイルに合うように変えてね
} else if (i < 100) {
    txt_name <- paste0("hist00", i, ".csv") #ここを自分の結果のファイルに合うように変えてね
    png_name <- paste0("_hist00",i,".png")#ここを自分の結果のファイルに合うように変えてね
} else if (i < 1000) {
    txt_name <- paste0("hist0", i, ".csv") #ここを自分の結果のファイルに合うように変えてね
    png_name <- paste0("_hist0",i,".png")#ここを自分の結果のファイルに合うように変えてね
  }
  txt_data <- read.table(txt_name, fileEncoding = "UTF-8", sep = ',')
#if (i == 1) {
  png(png_name, width = 360, height = 360)
  #par(mar = c(5, 5, 5, 10)) 
  hist(x = txt_data[,1], breaks = seq(0, 200, by = 20),main= txt_name, xlab="angle", ylab="frequency",col="aquamarine3")
  par(xpd = T)
  #legend(par()$usr[2] + 0.6, par()$usr[4], legend = c(""), pch = c(4), col = c(4))
  dev.off()

}

