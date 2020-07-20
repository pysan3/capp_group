# capp_group

Cアプリケーションデベロップメントグループ課題

## 内容

雪合戦をするゲーム。プレイヤーは動くことができ、玉を投げて敵に当てる。また敵は自動で玉を投げてくるが、プレイヤーはシールドを出すことによって自分を守ることができるが、玉が当たるとHPが減ってしまう。自分のHPがなくなったら相手の勝ち、相手のHPがなくなったら自分の勝ちになる。時間切れになったら残りHPが高い方の勝ち。

## Usage

```bash
mkdir build
cd build
cmake ..
make
```

## Requirements

* gcc
* OpenGL
