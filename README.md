### Meeting-Matting
视频会议抠背景，基本功能类似于腾讯会议、Google Meeting

最近因为工作需要，Google Meeting用的比较多，用的时候我就发现几个问题了。1. 人像抠图效果一般；2. 虚拟背景违和感强。每次开摄像头面对面谈东西的时候，总是看着对方的虚拟背景看着看着就出戏了，实在是太假了。

因此萌发了做一个视频会议换背景的小项目。目前人像抠图用的是MODNet，调色用的是SSH。

PS：项目中的网络推理均使用ncnn引擎实现

#### 功能
1. 人像视频换背景：使用[MODNet](https://github.com/ZHKKKe/MODNet)的人像抠图实现，直接用的[飞哥传书](https://github.com/FeiGeChuanShu/ncnn_Android_matting)转换好的模型。目前问题就是慢，在我的电脑上只能跑到2fps，我以前也做过抠图模型，能跑到20fps左右，但是效果比较差，就不用了。
2. 人像&背景调色：使用[SSH](https://github.com/VITA-Group/SSHarmonization)实现，SSH是个对合成图像进行调色的工作，具体可以看[文章](https://zhuanlan.zhihu.com/p/414874339)。目前问题就一点，调出来的颜色像僵尸┑(￣Д ￣)┍。

#### 开发记录
- [x] 人像抠图
- [x] 色调调整

#### 效果
目前效果稀烂，不堪入目，等我挑到好看的效果图再传上来

#### 改进点
1. 换更好的抠图模型（有卡再说）
2. 换更好的调色模型（有卡再说）
3. 增加锁头效果(类似苹果的那种,通过裁切画面保证人在中间)（有1080P摄像头再说）
4. 氪金

### 参考
1. https://github.com/Tencent/ncnn
2. https://github.com/FeiGeChuanShu/ncnn_Android_matting
3. https://github.com/VITA-Group/SSHarmonization