# Changelog

## [1.5.0](https://github.com/glowbuzzer/gbem/compare/v1.4.0...v1.5.0) (2024-02-15)


### Features

* **#100:** Add ini file for debug config ([6ca0bfa](https://github.com/glowbuzzer/gbem/commit/6ca0bfa51c5ef2ab292fee00ed4f8b170a8eb652))
* **#101:** Add EL6021 RS485 support ([b2595b9](https://github.com/glowbuzzer/gbem/commit/b2595b914deaab35ad4abedf5de62f3925118986))
* **#102:** Create single AW-6DOF and AW-6DOF-FSOE machine maps ([fe27328](https://github.com/glowbuzzer/gbem/commit/fe27328102f330218d7c660b4c09c7d57900d57c))
* **#102:** Create single AW-6DOF and AW-6DOF-FSOE machine maps ([e33e01a](https://github.com/glowbuzzer/gbem/commit/e33e01a62c5b41d8f5cc40e817f95dc71430d606))
* **#105:** Add ability to have optional slaves in machine config ([946e9d9](https://github.com/glowbuzzer/gbem/commit/946e9d9e9d4483d6d80bf67e29b79719f38e8038))
* **#71:** Add FSoE handling for AW J series drives ([9563062](https://github.com/glowbuzzer/gbem/commit/95630626e7350d0659b227a236ac8646dfe6397f))
* **#72:** Add support for BBH SCU-1-EC ([6a2ffe2](https://github.com/glowbuzzer/gbem/commit/6a2ffe25cd77893a954403ce473022331e4de655))
* **#72:** Add support for BBH SCU-1-EC ([9c8c7c2](https://github.com/glowbuzzer/gbem/commit/9c8c7c2efaaefb56e55bc4b188ea5360738492a0))
* **#72:** Add support for BBH SCU-1-EC ([06f8a07](https://github.com/glowbuzzer/gbem/commit/06f8a072d997997ef1108641a66f553d78f40e5f))
* **#73:** Add support for Beckhoff EL6900, EL1904 and EL2904 ([5b4b572](https://github.com/glowbuzzer/gbem/commit/5b4b572efdf69cb41d7456e6de21405213bb1152))
* **#75:** Add test machines for EL600, SCU, Synapticon FSoE ([867fbd8](https://github.com/glowbuzzer/gbem/commit/867fbd8925d7960bda86837467d906a2c412c980))
* **#75:** Add test machines for EL600, SCU, Synapticon FSoE ([ae4395f](https://github.com/glowbuzzer/gbem/commit/ae4395f1cfa64eda7c6430118c236f403b84295d))
* **#76:** Add custom SM/FMMU function ([9175c0d](https://github.com/glowbuzzer/gbem/commit/9175c0de7ea410cccc682ba0db635877dba63f4d))
* **#77:** Add core FSoE copying ([10ebf51](https://github.com/glowbuzzer/gbem/commit/10ebf51a2f72d4def0d7ac47195c6aaa3db49356))
* **#77:** Add core FSoE copying ([3e4d7a7](https://github.com/glowbuzzer/gbem/commit/3e4d7a71fa3922327ebb3df6dc535738ca9dda6e))
* **#79:** Add Tool Outputs to IO map for AW tube robots ([56579e0](https://github.com/glowbuzzer/gbem/commit/56579e0d39664cf0275f321ac8abcba698175013))
* **#80:** Add history to drive faults so they show current and last ([ec39b4e](https://github.com/glowbuzzer/gbem/commit/ec39b4ece5f2c86d2daafb983563f118148c0e7f))
* **#80:** Add history to drive faults so they show current and last ([0c15d7b](https://github.com/glowbuzzer/gbem/commit/0c15d7b14fa3412c420cf4af6a54eaf8e33b5634))
* **#80:** Add Tool Outputs to IO map for AW tube robots ([fefe9f7](https://github.com/glowbuzzer/gbem/commit/fefe9f71f5e30d923843c8e6eaff22fd29a166e8))
* **#81:** Update synapticon error table to include new errors ([d635455](https://github.com/glowbuzzer/gbem/commit/d63545537145a616f2ff865ad8fcf6c75085c19e))
* **#82:** Add FSoE config to all machine configs even if unused ([5fc1008](https://github.com/glowbuzzer/gbem/commit/5fc100829c9bd8af975d2ac626713c939c5dad22))
* **#84:** Add estop function to machine configs rather than simple DIN to handle more complex FSoE scenario ([01bbf70](https://github.com/glowbuzzer/gbem/commit/01bbf70ecaa32618587c54993b0090f900cf93ea))
* **#85:** Add estop function to machine configs rather than simple DIN to handle more complex FSoE scenarios ([90dcd85](https://github.com/glowbuzzer/gbem/commit/90dcd85e3bb7c4604205264ce733bde2af799493))
* **#86:** Add AW J-series custom PDO map with LED control (for FSoE) ([33c5b63](https://github.com/glowbuzzer/gbem/commit/33c5b631e4e0e75160c3a7e483089e095647d158))
* **#86:** Process AW J Series drive error messages from PDO not SDO ([5f11106](https://github.com/glowbuzzer/gbem/commit/5f111063179290f69449cb9dd1fa526e0a4a7865))
* **#90:** Add FoE upload for synapticon logging_curr.log to GBEM ([0465317](https://github.com/glowbuzzer/gbem/commit/046531751d58c1f09735e84e9baa112f914e295b))
* **#92:** Add ini file for drive limits ([8123e93](https://github.com/glowbuzzer/gbem/commit/8123e93fc75793f5ce08a99ddb5a5434a4d3b5df))
* **#92:** Add ini file for drive limits ([9b05e70](https://github.com/glowbuzzer/gbem/commit/9b05e70352c42b8c123400d162e5ea3204f58718))
* **#92:** Add ini file for drive limits ([ffa7223](https://github.com/glowbuzzer/gbem/commit/ffa7223a9eaf9a1e1fbf7d25636464317107c5f4))
* **#93:** Add internal limit active (current and history) to status ([4be8526](https://github.com/glowbuzzer/gbem/commit/4be8526f59fb01e71b7923437b000abc027e44d8))
* **#94:** Change AW J Series custom PDO map - remove tuning and analogs and add control effort and error codes ([81a479f](https://github.com/glowbuzzer/gbem/commit/81a479f545ce089fda8bebd510ef3535c44ff9bd))
* **#95:** Consolidate AW J Series SDO setting from different joint sizes J17, J20, J25 et ([a491c19](https://github.com/glowbuzzer/gbem/commit/a491c19d1ca81d257c7555aefa9d55c4aef718ef))
* **#96:** Process AW J Series drive error messages from PDO not SDO ([5b40061](https://github.com/glowbuzzer/gbem/commit/5b4006181326baed0f399ec69a4a7b82345752c1))
* **#97:** Make internal limit active detection for AW J Series have to occur for multiple cycles to trigger error ([24744ae](https://github.com/glowbuzzer/gbem/commit/24744aebe9b78e35ce0d0a8f2aad8aa816dde671))
* **#98:** Add control effort to Telemetry for AW J Series drives ([e52c079](https://github.com/glowbuzzer/gbem/commit/e52c07918a7fadce6454c276117446ccfe99a934))
* **#98:** Add control effort to Telemetry for AW J Series drives ([19c7248](https://github.com/glowbuzzer/gbem/commit/19c7248e3b7661d8767fee1c5056c586bff0456c))
* **#98:** Add control effort to Telemetry for AW J Series drives ([3fe478f](https://github.com/glowbuzzer/gbem/commit/3fe478f42d98715d6593ff3297ad23e360823ed2))
* **#99:** Add new digital io type - safety ([9885a6d](https://github.com/glowbuzzer/gbem/commit/9885a6d59be103e2036d4da04a6b620ba1075ab2))
* **#99:** Add new digital io type - safety ([184361f](https://github.com/glowbuzzer/gbem/commit/184361ff61a099452c557c799b5adc6d07672e67))


### Bug Fixes

* **#104:** Boot can get stuck in GBEM: Boot step 3 &gt;failure&lt; (transition all slaves to PRE OP state) ([93633d2](https://github.com/glowbuzzer/gbem/commit/93633d2f0b47826eea4fb9c7698128f905b3e81c))

## [1.4.0](https://github.com/glowbuzzer/gbem/compare/v1.3.0...v1.4.0) (2023-12-16)


### Features

* **#60:** Add support for 2 estop (quickstop DINs) ([098d18d](https://github.com/glowbuzzer/gbem/commit/098d18d33bc4ee1b71ffa42d6c4510cfbfe891bd))
* **#63:** Make joint limits in config degrees not encoder ticks ([299fe8a](https://github.com/glowbuzzer/gbem/commit/299fe8a98d8c74e4774bd4efa55ca84af3942cbc))
* **#64:** Move standard SDOs for AW drives into common code ([7b36b2f](https://github.com/glowbuzzer/gbem/commit/7b36b2f64bfa93a8adab4889e3b53144f3b85c7e))
* **#65:** Add torque and velocity controllers ([c5c3414](https://github.com/glowbuzzer/gbem/commit/c5c3414003ed881c25998f5f64b3e9b8869ade50))
* **#66:** If incorrect slaves found, restart not exit ([5373178](https://github.com/glowbuzzer/gbem/commit/5373178287cd44984f8301562849584b07a7ce18))
* **#67:** Add torque_offset handling ([19ba334](https://github.com/glowbuzzer/gbem/commit/19ba33479906819db6d331e9cb00d2daef8e76ec))
* **#67:** Add torque_offset handling ([6d00fee](https://github.com/glowbuzzer/gbem/commit/6d00fee9695644c0ab58ecb12fa4b0a0d925fb99))
* **#68:** Add support for velocity offset ([2a41746](https://github.com/glowbuzzer/gbem/commit/2a417460bb8d7ac794e5a70028d02dd830d9c863))
* **#68:** Add support for velocity offset ([993e36b](https://github.com/glowbuzzer/gbem/commit/993e36be4a2162568f4b02d8258ebe7871d8ead6))
* **#69:** Add support for CST/CSV position monitoring ([904b586](https://github.com/glowbuzzer/gbem/commit/904b586b4e08ae648e930c47fa77cdf300870835))
* **#70:** Add AW L robot machine map ([a128962](https://github.com/glowbuzzer/gbem/commit/a12896203cbc69565fd68807510d5c4b0a9b1ca0))

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
