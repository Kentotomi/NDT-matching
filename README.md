# NDT-matching
ロボットシステム学課題3

# DEMO
## デモ動画
https://youtu.be/PsEV-1tdwX4
## 結果
色付きが地図データ，白がスキャンデータ
### 実行前
![robosys1](https://user-images.githubusercontent.com/68839397/105617054-636cbd80-5e1f-11eb-8395-409f5e0d55b5.png)
### 実行後
![robosys2](https://user-images.githubusercontent.com/68839397/105617083-c3636400-5e1f-11eb-8688-974f5a4b1fcc.png)


# Features
3次元地図の点群データと3次元のスキャンデータ(点群)をNDTスキャンマッチングさせるプログラム．
どの程度マッチグしているかを評価するスコア関数と探索からマッチング終了までの時間を表示している．
必要に応じてスキャンデータに対して初期位置と姿勢を与え，マッチングにかかる時間を短くすることができる．

# Requirement
## 環境
- OS: Ubuntu 20.04.1 LTS
## データの撮影環境
### センサ
- realsense d435i
### 撮影環境
- 千葉工業大学 津田沼キャンパス内
- 撮影距離 約25 m
# Usage

```
$git clone https://github.com/Kentotomi/NDT-matching.git
$cd NDT-matching
```
地図データとスキャンデータをどうディレクトリ内に置く．
必要であればスキャンデータに初期位置を与える．
```
$cmake .
$make
$./normal_distributions_transform
```
# License
"normal_distributions_transform.cpp" is under [The 3-Clause BSD License](https://opensource.org/licenses/BSD-3-Clausee)
