# Changelog

## [1.3.0](https://github.com/glowbuzzer/gbem/compare/v1.2.0...v1.3.0) (2023-10-02)


### Features

* **#11:** Add change markers - NEW_MACHINE, MACHINE_SWAP, NEW_DRIVE ([95dac8f](https://github.com/glowbuzzer/gbem/commit/95dac8fc15f249e63261966caa8db3d617660ddf))
* **#12:** Merge SOEM slaveinfo code changes ([81ef345](https://github.com/glowbuzzer/gbem/commit/81ef345cbfcc6a7b675b7d14d1014900c1bff7ad))
* **#18:** Add print slave error message when in fault ([754d1ec](https://github.com/glowbuzzer/gbem/commit/754d1ecefd91209705fac673aa69a577070e860d))
* **#20:** Add AutomationWare Stewart platform machine config and drive support ([96dd060](https://github.com/glowbuzzer/gbem/commit/96dd060535a48ee0100f9ffb7a73c03d4b7f99c7))
* **#21:** Add feature to catch signals and clean up on exit ([bab4bc8](https://github.com/glowbuzzer/gbem/commit/bab4bc899deda81a47d852755237b0eae19f9a18))
* **#22:** Add boot and homing command / status flags ([b100a7e](https://github.com/glowbuzzer/gbem/commit/b100a7eace8e07005cb4c8e63769e7d09d49535f))
* **#23:** Add ethtool nic tuning command to cmake ([8a46567](https://github.com/glowbuzzer/gbem/commit/8a4656741aadadea561f32ff6093e321e6380463))
* **#24:** Remove const from AZD drivers ([704d1f2](https://github.com/glowbuzzer/gbem/commit/704d1f276a7c4e0563c55432bc9067860ddf0a4a))
* **#25:** Add SOCK_NONBLOCK to nic driver and move ecx_outframe_red ([5e33530](https://github.com/glowbuzzer/gbem/commit/5e335308ba9e3d14e06b57830c295d2b201d1cfc))
* **#26:** Rework osal timer functions to use CLOCK_REALTIME ([aaf498c](https://github.com/glowbuzzer/gbem/commit/aaf498cc697009cf8afa4a40fc9063761104adc3))
* **#27:** Change ecx_FPWR timeout in slave mailbox write ([f2dfa14](https://github.com/glowbuzzer/gbem/commit/f2dfa14ce58077d1238ce6d41d44ffa8baa7fdc0))
* **#28:** Support use of SIGRTMIN ([c2b493c](https://github.com/glowbuzzer/gbem/commit/c2b493c37dd04bd48b4ab6675a1f482068004338))
* **#28:** Support use of SIGRTMIN ([ff50e42](https://github.com/glowbuzzer/gbem/commit/ff50e4204b4d72378ca653c5b30af41c5e3ae155))
* **#29:** Add signalling of boot status to GBC ([dc97b56](https://github.com/glowbuzzer/gbem/commit/dc97b56e7f0763f1436353b1847b1debe1d0dd07))
* **#30:** Add define ENABLE_CYCLIC_MESSAGES, to control user message o/p ([4ec400b](https://github.com/glowbuzzer/gbem/commit/4ec400bec060a40fcfe73927b966b73e382a0f3c))
* **#31:** Add USE_CLOCK_DIFFERENCE define to control whether ecrxtx uses the local to DC clock difference or not ([06d1daa](https://github.com/glowbuzzer/gbem/commit/06d1daa3a0316dc65b7956a340005adb44d7c7f0))
* **#32:** Add a variable to the ecm_status struct to track shared mem busy events - shared_mem_busy_count ([7970bf2](https://github.com/glowbuzzer/gbem/commit/7970bf284ccbf7578ddd45db41b80fa9704ff540))
* **#33:** Add clean up code to force state to EC_STATE_INIT on exit ([ff106ac](https://github.com/glowbuzzer/gbem/commit/ff106ac65c73548493b8e057d48fdeec9860902c))
* **#34:** Move read drive status messages (when in error state) into separate thread ([e9e311b](https://github.com/glowbuzzer/gbem/commit/e9e311b1e0060b208cbdffb64dd97067747c1305))
* **#35:** Use a separate thread for the reboot process ([b773e0a](https://github.com/glowbuzzer/gbem/commit/b773e0a1a0533d8b24b9dbcdbca45396c2c076c6))
* **#37:** Merge SOEM core code changes 21-&gt;23 ([ea6cae6](https://github.com/glowbuzzer/gbem/commit/ea6cae67a9171ce56e8b89f9c0909a7013bb5700))
* **#39:** Remove test projects from cmake ([f4534d0](https://github.com/glowbuzzer/gbem/commit/f4534d0052e80aaf62bca843ff81eecfb0988551))
* **#41:** Move ecm_status lib from GBEM to GCLIBS ([fa473a3](https://github.com/glowbuzzer/gbem/commit/fa473a3878a5ba9bcb300afac111c1052ca117e8))
* **#43:** Replace signal with named semaphore for shared mem and trigger ([804a3b4](https://github.com/glowbuzzer/gbem/commit/804a3b442c1733be6c512cda31a020305474a706))
* **#44:** Add support for CST and CSV drive control ([ae6d04d](https://github.com/glowbuzzer/gbem/commit/ae6d04d6b4c913c789b9c35e46385aca9b579243))
* **#44:** Add support for CST and CSV drive control ([0f0b4d3](https://github.com/glowbuzzer/gbem/commit/0f0b4d39f5a03b3fc55dc3522858df11c60a1e55))
* **#44:** Add support for CST and CSV drive control ([eb099ee](https://github.com/glowbuzzer/gbem/commit/eb099ee95a856e11b1c65bab4b2fdd80f93a3450))
* **#44:** Add support for CST and CSV drive control ([85a571b](https://github.com/glowbuzzer/gbem/commit/85a571bad2247e5981dd9c52528df4ba9a711cbb))
* **#45:** Add test function to print contents of ecm_status struct ([a1c7a99](https://github.com/glowbuzzer/gbem/commit/a1c7a9971135418960eb6f4ab1a0a0f7bdca3b8c))
* **#46:** ecm_status does not contain error messages from drives with a PDO error function ([bdc697f](https://github.com/glowbuzzer/gbem/commit/bdc697f84065a97e5a4b73e8dfd9e07605de71d8))
* **#47:** Add CSV support to virtual drives ([cf5cb1b](https://github.com/glowbuzzer/gbem/commit/cf5cb1bb44baa68b44783aa94369a1e15e70ba54))
* **#49:** Remove emstat json config maker as functionality is now in GBR ([fb2350a](https://github.com/glowbuzzer/gbem/commit/fb2350a5e9ada1b66aa512c5b22d2b7f5f2c56f1))
* **#50:** Add custom PDO mapping to Oriental Motor AZDXA driver to include act veclocity ([a018761](https://github.com/glowbuzzer/gbem/commit/a018761140e2bd6456a7938bc85a2f98a194a71c))
* **#51:** Add basic P and PIV controller for CSV mode ([fb5b857](https://github.com/glowbuzzer/gbem/commit/fb5b85734520a99b8e8743dc27cc5da019eaa68d))
* **#53:** Add option to NOT log error messages with SDO reads and writes ([a3045b6](https://github.com/glowbuzzer/gbem/commit/a3045b62423f98d1a7c2e36fbfdac6e8c7ee87a1))
* **#53:** Add option to NOT log error messages with SDO reads and writes ([b4d3ecb](https://github.com/glowbuzzer/gbem/commit/b4d3ecbfd616d58626c5324b43b8f9c02dbcd9f4))
* **#54:** Extend CSV support for AZD drives to 2A and 4A models ([d218d43](https://github.com/glowbuzzer/gbem/commit/d218d438056af3336ea50ccceb2b22a8e9d70dd0))
* **#55:** Allow PLC cycle times for tasks above 254ms ([fab5223](https://github.com/glowbuzzer/gbem/commit/fab5223edc4c5eeb1236070f03500883b13519a6))
* **#56:** Add AW robot skeleton PLC task to control safety PLC ([fe6c218](https://github.com/glowbuzzer/gbem/commit/fe6c218ee7475d5efdc506fd45af56a81bfc8a6f))
* **#57:** Copy ecm_status data when in all CIA-402 states ([d534389](https://github.com/glowbuzzer/gbem/commit/d5343890ce2656b43c55994a4a70011eb2d4ae66))
* **#58:** Use named_offline_mem_protection_semaphore to protect shared memory for ecm_status transfer ([21caad0](https://github.com/glowbuzzer/gbem/commit/21caad0764028390f8c31faa17825db8d4a46167))
* Add ecm_status to GCLIBS ([bd6ea89](https://github.com/glowbuzzer/gbem/commit/bd6ea89fcf15d58023521ed10a79aff5e9547b3a))


### Bug Fixes

* **#36:** Missing file status_control_word_bit_definitions.h ([89614be](https://github.com/glowbuzzer/gbem/commit/89614be417d6dd7dc180d9537c31b61d588f0877))
* **#40:** Homing and status commands don't persist pass boot stage ([10d96e8](https://github.com/glowbuzzer/gbem/commit/10d96e81914d5941088069d9570c6e8393972dd5))

## [1.2.0](https://github.com/glowbuzzer/gbem/compare/v1.1.0...v1.2.0) (2023-09-04)


### Features

* **#17:** Add ENABLE_PLC #define to enabled/disable PLC functionality ([9f44e40](https://github.com/glowbuzzer/gbem/commit/9f44e40d6b403a43b61b287dcec1b9112773e799))


### Bug Fixes

* **#15:** GBEM doesn't respond to CTRL_GBC_INTERNAL_FAULT_REQ_BIT_NUM in control word ([a6cc9db](https://github.com/glowbuzzer/gbem/commit/a6cc9db7837a3c6c5a2ad6f308076726d67f542a))

## [1.1.0](https://github.com/glowbuzzer/gbem/compare/v1.0.0...v1.1.0) (2023-08-31)


### Features

* **#8:** add nolimits command line flag to disable drive limits ([3592108](https://github.com/glowbuzzer/gbem/commit/35921088d4b10682d045ceb33e0e5992546eec47))
* **#9:** Add #define to control execution time monitoring ([5259315](https://github.com/glowbuzzer/gbem/commit/5259315dd8f55ee11471e7d1c68cd474560a22c1))
* FATAL error handler shows file name ([f564ddd](https://github.com/glowbuzzer/gbem/commit/f564ddd13766fb16fe89fd8a4f8809d3d57377db))
* FATAL error handler shows file name ([fd75815](https://github.com/glowbuzzer/gbem/commit/fd758158f8f022c35539f5934d1363f72d407256))

## 1.0.0 (2022-07-20)


### Features

* **#2:** Slave name and other checking does not work ([d0e55aa](https://github.com/glowbuzzer/gbem/commit/d0e55aaaea74223821dd00c245bba21c7bf607e7))
* Added git tage version ([8703326](https://github.com/glowbuzzer/gbem/commit/870332697e3a1a7300d0080e80e8a9fcbc163f11))


### Bug Fixes

* **#5:** Transition from fault reaction to fault doesn't work ([04d3643](https://github.com/glowbuzzer/gbem/commit/04d3643dc762eadaf86ab9dbe2d80648508f0f49))
