# μT-Kernel 3.0 BSP (Board Support Package)
μT-Kernel 3.0 BSP is a T-Kernel 3.0 development environment package for various development boards.  
A set of μT-Kernel 3.0, device drivers, and development environment projects configured for each hardware.  

μT-Kernel 3.0 BSPは、各種開発ボード用のμT-Kernel 3.0の開発環境のパッケージです。  
それぞれのハードウェアに向けて設定されたμT-Kernel 3.0とデバイスドライバ、そして開発環境のプロジェクトが一式となっています。  

Please download from the following branch for each development board.  
以下の開発ボード毎のブランチからダウンロードしてください。

| Branch | CPU | Board | Development environment |
| --- | ----------------------- | ------ | -------- |
| ewarm-sbk_m4kn | TMPM4KNFYAFG | SBK-M4KN(ES) | EWARM |
| nucleo_stm32l476 | STM32L476 | Nucleo-64 | STM32CubeIDE |
| nucleo_stm32h723 | STM32H723 | Nucleo-144 | STM32CubeIDE |
| rtb_rx65n | RX65N | Renesas Target Board | e2Studio |
| rsk_rx65n | RX65N | Renesas Starter Kit+ | e2Studio |
| pico_rp2040 | RP2040 | Raspberry Pi Pico | Eclipse CDT |
---

## ライセンス

μT-Kernel 3.0は、トロンフォーラムがT-License2.2に従って配布するソースコードです。

T-License2.2の内容は以下をご覧ください。

- /docs/TEF000-219-200401.pdf

## 追加ソースコード

本リポジトリに含まれるソースコードは、T-License2.2に規定された再配布の条件に従ってユーシーテクノロジ株式会社(以下、UCT)が無償で再配布しています。

本リポジトリの以下のブランチには、UCTが開発したソースコードが含まれます。

- ewarm-sbk_m4kn : SBK-M4KN用ボードサポートパッケージ

UCTは、UCTが開発したソースコードを含めて、本リポジトリに含まれるソースコードについてT-License2.2第３条第3項に定める利用を認めます。ただし、UCTが開発したソースコードについて、T-License2.2第３条第3項の第４号、第５号における有償での再配布は認めません。


## 注意事項

- 本リポジトリに含まれるソースコード(付随するコメント、ドキュメンテーションを含む)を「本ソースコード」といいます。
- 本ソースコードのうちμT-Kernel 3.0の部分についてはT-License2.2が適用されることを確認します。
- 本ソースコードは全て無保証で提供するものであり、その適用可能性も含めて、UCTはいかなる保証も行いません。
- 本ソースコードを利用することにより直接的または間接的に生じたいかなる損害に関しても、UCTは一切の責任を負いません。
- 本ソースコードの仕様、実装にいかなる欠陥が発見されたとしても、UCTには対応義務はありません。
- 本ソースコードの仕様、実装は予告なく変更されることがあります。
- 本ソースコードの仕様、実装の変更に伴って利用者にいかなる損害が発生したとしても、UCTは一切の責任を負いません。
- 本ソースコードに対するライセンス料、保守費用、ロイヤリティは不要です。
- 本ソースコードに関するお問い合わせは一切お受けできません。お問い合わせいただいても回答することはありません。
