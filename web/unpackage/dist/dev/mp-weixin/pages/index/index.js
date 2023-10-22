"use strict";
const common_vendor = require("../../common/vendor.js");
const _sfc_main = {
  data() {
    return {
      loadBluetoothStatus: "more",
      loadBluetoothcolor: "#007AFF",
      // statusTypes: [
      //   {
      //     value: 'more',
      //     text: '加载前',
      //     checked: true,
      //   },
      //   {
      //     value: 'loading',
      //     text: '加载中',
      //     checked: false,
      //   },
      //   {
      //     value: 'noMore',
      //     text: '没有更多',
      //     checked: false,
      //   },
      // ],
      contentText: {
        contentdown: "点击连接蓝牙",
        contentrefresh: "蓝牙加载中",
        contentnomore: "蓝牙加载失败"
      }
    };
  },
  onLoad() {
  },
  methods: {
    leftButton() {
    },
    rightButton() {
    },
    topButton() {
    },
    bottomButton() {
    },
    loadBluetooth() {
      this.loadBluetoothStatus = "loading";
      common_vendor.index.openBluetoothAdapter({
        success(res) {
          console.log("初始化蓝牙成功");
          console.log(res);
        },
        fail(err) {
          console.log("初始化蓝牙失败");
          console.error(err);
        }
      });
    }
  }
};
if (!Array) {
  const _easycom_uni_load_more2 = common_vendor.resolveComponent("uni-load-more");
  _easycom_uni_load_more2();
}
const _easycom_uni_load_more = () => "../../uni_modules/uni-load-more/components/uni-load-more/uni-load-more.js";
if (!Math) {
  _easycom_uni_load_more();
}
function _sfc_render(_ctx, _cache, $props, $setup, $data, $options) {
  return {
    a: common_vendor.o(($event) => $options.leftButton()),
    b: common_vendor.o(($event) => $options.rightButton()),
    c: common_vendor.o(($event) => $options.topButton()),
    d: common_vendor.o(($event) => $options.bottomButton()),
    e: common_vendor.p({
      iconType: "circle",
      status: $data.loadBluetoothStatus,
      ["content-text"]: $data.contentText,
      color: $data.loadBluetoothcolor
    }),
    f: common_vendor.o(($event) => $options.loadBluetooth())
  };
}
const MiniProgramPage = /* @__PURE__ */ common_vendor._export_sfc(_sfc_main, [["render", _sfc_render], ["__file", "/Users/YellowWenJie/Desktop/stm32_study/web/pages/index/index.vue"]]);
wx.createPage(MiniProgramPage);
