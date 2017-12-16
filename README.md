# VLSI_Project
Final Project for VLSI course.

##parser的使用方法
./parser [输入文件名] [输出文件名] [输出映射表文件名]
如./parser ibmpg1.spice ibmpg1.out ibmpg1.map
###输出文件格式
第一行一个数ｎ表示有多少个点
接下来n行每行两个数I_{i}和G_{i}，分别表示流入i点的电流源电流和以及ｉ点的自电导。
接下来一行一个数ｆ，表示电压已经被确定了的点的个数
接下来ｆ行，每行两个数i和f_{i}，表示ｉ点的编号以及确定的电压f_{i}
接下来一行一个数m，表示边数
接下来ｍ行，每行三个数u,v,w，表示ｕ和ｖ之间的电导为ｗ，（u,v和v,u会出现两遍）
###映射文件格式
map文件就是原始文件里点的名字和输出的编号的映射，多个点同一个编号是因为它们电压相同
