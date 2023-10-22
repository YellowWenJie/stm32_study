if (typeof Promise !== "undefined" && !Promise.prototype.finally) {
  Promise.prototype.finally = function(callback) {
    const promise = this.constructor;
    return this.then(
      (value) => promise.resolve(callback()).then(() => value),
      (reason) => promise.resolve(callback()).then(() => {
        throw reason;
      })
    );
  };
}
;
if (typeof uni !== "undefined" && uni && uni.requireGlobal) {
  const global = uni.requireGlobal();
  ArrayBuffer = global.ArrayBuffer;
  Int8Array = global.Int8Array;
  Uint8Array = global.Uint8Array;
  Uint8ClampedArray = global.Uint8ClampedArray;
  Int16Array = global.Int16Array;
  Uint16Array = global.Uint16Array;
  Int32Array = global.Int32Array;
  Uint32Array = global.Uint32Array;
  Float32Array = global.Float32Array;
  Float64Array = global.Float64Array;
  BigInt64Array = global.BigInt64Array;
  BigUint64Array = global.BigUint64Array;
}
;
if (uni.restoreGlobal) {
  uni.restoreGlobal(Vue, weex, plus, setTimeout, clearTimeout, setInterval, clearInterval);
}
(function(vue) {
  "use strict";
  function formatAppLog(type, filename, ...args) {
    if (uni.__log__) {
      uni.__log__(type, filename, ...args);
    } else {
      console[type].apply(console, [...args, filename]);
    }
  }
  new Proxy({}, {
    get(_, key) {
      throw new Error(`Module "assert" has been externalized for browser compatibility. Cannot access "assert.${key}" in client code.  See http://vitejs.dev/guide/troubleshooting.html#module-externalized-for-browser-compatibility for more details.`);
    }
  });
  const _export_sfc = (sfc, props) => {
    const target = sfc.__vccOpts || sfc;
    for (const [key, val] of props) {
      target[key] = val;
    }
    return target;
  };
  const _sfc_main$2 = {
    data() {
      return {
        //设备id
        deviceId: "",
        //服务id，这里根据自己的设备写死
        serviceId: "0000FFE0-8888-1000-8000-0088888888888",
        //特征值id-----这里根据自己的设备写死
        characteristicId: "0000FFE1-8888-1000-8000-88888888",
        //设备名称
        deviceName: "",
        //存放蓝牙设备数组
        devices: [],
        //查看是否连接
        connected: false,
        //存储当前连接状态，里面的value就是手机接收的值
        chs: [],
        //记录电脑向手机传递的数据
        myDataMeasure: [],
        //查询蓝牙适配器的开始时间
        _discoveryStarted: false,
        //是否可写
        canWrite: false,
        //要发送的数据
        sendData: "",
        //下部信息条的显示与隐藏(默认显示)
        infoShowHide: true,
        //隐藏和打开按钮的字
        infoShowHideButton: "隐藏",
        //用户对设备设置的基本参数
        myData: {},
        //临时存储仪器传输的数据以及基本设置信息
        myMeatureDataPhone: [],
        //存放数据的key，等待网络好的时候通过key传递数据到服务器
        myTemplateKey: [],
        //记录存储数据的时间
        timeNow: ""
      };
    },
    created() {
    },
    onPullDownRefresh() {
      setTimeout(() => {
        formatAppLog("log", "at pages/index/index.vue:146", "VUEX数据", this.$store.state.myDataGlobal);
        formatAppLog("log", "at pages/index/index.vue:147", "VUEX数据时间间隔", this.$store.state.myDataGlobal.myInterval);
        formatAppLog("log", "at pages/index/index.vue:148", "实际蓝牙数据", this.myData);
        formatAppLog("log", "at pages/index/index.vue:149", "实际蓝牙输入框数据", this.sendData);
        uni.showToast({
          title: "参数更新成功"
        });
        uni.stopPullDownRefresh();
      }, 1e3);
    },
    watch: {
      "$store.state.myDataGlobal": {
        deep: true,
        immediate: true,
        handler() {
          this.myData = this.$store.state.myDataGlobal;
          this.sendData = (Number(this.myData.myInterval) * 10).toString() + this.myData.myNumber;
        }
      }
    },
    mounted() {
    },
    methods: {
      //发送存储中数据
      sendStorage() {
        uni.showLoading({
          title: "数据发送中..."
        });
        uni.getStorage({
          key: "mykey",
          success: (res) => {
            this.myTemplateKey = res.data;
            formatAppLog("log", "at pages/index/index.vue:181", "这是来自手机内存中的key", this.myTemplateKey);
          },
          fail: () => {
            formatAppLog("log", "at pages/index/index.vue:184", "key不存在，无法发送");
            this.myTemplateKey = [];
            return;
          }
        });
        formatAppLog("log", "at pages/index/index.vue:190", "数据存储的key：", this.myTemplateKey);
        for (let i = 0; i < this.myTemplateKey.length; i++) {
          formatAppLog("log", "at pages/index/index.vue:193", `第${i}个:`, i);
          uni.getStorage({
            key: this.myTemplateKey[i],
            //前后两次存的文件也都能找得到
            success: (res) => {
              formatAppLog("log", "at pages/index/index.vue:198", `打印存储在storage中${i}的数据`, res.data);
            },
            fail: () => {
              formatAppLog("log", "at pages/index/index.vue:201", "该数据不存在");
            }
          });
        }
        setTimeout(() => {
          uni.hideLoading();
          uni.showToast({
            title: "查询并发送成功"
          });
        }, 1e3);
      },
      //为了去除重复的
      inArray(arr, key, val) {
        for (let i = 0; i < arr.length; i++) {
          if (arr[i][key] === val) {
            return i;
          }
        }
        return -1;
      },
      //清空本地存储
      clearLocalStorage() {
        uni.showModal({
          title: "警告",
          content: "确认删除本地数据?请确认数据已及时发送",
          success: function(res) {
            if (res.confirm) {
              uni.clearStorage();
              uni.showToast({
                title: "本地数据已清空"
              });
            } else if (res.cancel) {
              formatAppLog("log", "at pages/index/index.vue:234", "用户点击取消");
              return;
            }
          }
        });
      },
      //将本次接受的数据保存
      saveStorage() {
        let date = /* @__PURE__ */ new Date();
        this.timeNow = date.getFullYear() + "-" + date.getMonth() + "-" + date.getDate() + " " + date.getHours() + ":" + date.getMinutes() + ":" + date.getSeconds();
        this.myMeatureDataPhone.push(this.timeNow);
        this.myMeatureDataPhone.push(this.myData.myInterval);
        this.myMeatureDataPhone.push(this.myData.myHomeStation);
        this.myMeatureDataPhone.push(this.myData.myOppositeStation);
        this.myMeatureDataPhone = this.myMeatureDataPhone.concat(this.myDataMeasure);
        formatAppLog("log", "at pages/index/index.vue:253", "设置参数及数据数组内容：", this.myMeatureDataPhone);
        formatAppLog("log", "at pages/index/index.vue:255", "本次的key:", this.myTemplateKey);
        uni.setStorage({
          key: this.timeNow,
          data: this.myMeatureDataPhone,
          success: () => {
            this.myTemplateKey.push(this.timeNow);
            uni.showToast({
              title: "数据保存成功"
            });
            this.myMeatureDataPhone = [];
          },
          fail: () => {
            uni.showToast({
              title: "数据保存失败",
              icon: "none"
            });
          }
        });
      },
      //将本组的数据进行保存
      saveGroup() {
        uni.setStorage({
          key: "mykey",
          data: this.myTemplateKey,
          success: () => {
            uni.showToast({
              title: "本组已保存"
            });
            formatAppLog("log", "at pages/index/index.vue:288", "key存储成功");
          }
        });
      },
      // ArrayBuffer转16进度字符串示例
      ab2hex(buffer) {
        var hexArr = Array.prototype.map.call(
          new Uint8Array(buffer),
          function(bit) {
            return ("00" + bit.toString(16)).slice(-2);
          }
        );
        return hexArr.join("");
      },
      //将16进制字符串转换为10进制
      hex2int(hex) {
        var len = hex.length, a = new Array(len), code;
        for (var i = 0; i < len; i++) {
          code = hex.charCodeAt(i);
          if (48 <= code && code < 58) {
            code -= 48;
          } else {
            code = (code & 223) - 65 + 10;
          }
          a[i] = code;
        }
        return a.reduce(function(acc, c) {
          acc = 16 * acc + c;
          return acc;
        }, 0);
      },
      //比大小
      myMax(n1, n2) {
        return Math.max(n1, n2);
      },
      //比长度
      myLen(arr) {
        arr = arr || [];
        return arr.length;
      },
      //显示隐藏下部信息条
      hideShowInfo() {
        this.infoShowHide = !this.infoShowHide;
        if (this.infoShowHideButton == "隐藏") {
          this.infoShowHideButton = "打开";
        } else {
          this.infoShowHideButton = "隐藏";
        }
      },
      //开启蓝牙适配器初始化蓝牙模块
      openBluetoothAdapter() {
        this.devices = [];
        uni.openBluetoothAdapter({
          success: (res) => {
            formatAppLog("log", "at pages/index/index.vue:346", "开启蓝牙适配器成功(openBluetoothAdapter success)", res);
            this.startBluetoothDevicesDiscovery();
            uni.showToast({
              title: "开始扫描设备",
              icon: "success"
            });
          },
          fail: (res) => {
            uni.showToast({
              title: "请开启蓝牙",
              icon: "none"
            });
            if (res.errCode === 10001) {
              uni.onBluetoothAdapterStateChange(function(res2) {
                if (res2.available) {
                  this.startBluetoothDevicesDiscovery();
                }
              });
            }
          }
        });
      },
      //开启蓝牙设备搜索
      startBluetoothDevicesDiscovery() {
        if (this._discoveryStarted) {
          return;
        }
        this._discoveryStarted = true;
        uni.startBluetoothDevicesDiscovery({
          allowDuplicatesKey: true,
          //允许重复上报同一个设备
          success: (res) => {
            formatAppLog("log", "at pages/index/index.vue:379", "开始搜寻蓝牙设备成功(startBluetoothDevicesDiscovery success)", res);
            uni.showLoading({
              title: "正在搜索设备"
            });
            this.onBluetoothDeviceFound();
          }
        });
      },
      //监听寻找到新设备的事件
      onBluetoothDeviceFound() {
        uni.onBluetoothDeviceFound((res) => {
          if (res) {
            uni.hideLoading();
          }
          res.devices.forEach((device) => {
            if (!device.name && !device.localName) {
              return;
            }
            const foundDevices = this.devices;
            const idx = this.inArray(foundDevices, "deviceId", device.deviceId);
            if (idx === -1) {
              this.devices.push(device);
            }
          });
          formatAppLog("log", "at pages/index/index.vue:405", this.devices);
        });
      },
      //【点击某个蓝牙设备时】将目前你点击的设备作为对象e传递进来
      createBLEConnection(e) {
        const ds = e.currentTarget.dataset;
        this.deviceId = ds.deviceId;
        this.deviceName = ds.name;
        uni.createBLEConnection({
          //这个deviceId不能删除，否则会报错【从这里往后要放一放】
          deviceId: this.deviceId,
          success: (res) => {
            this.connected = true, // __f__('log','at pages/index/index.vue:421',"设备已经连接(createBLEConnection)");
            formatAppLog("log", "at pages/index/index.vue:422", "连接时获取设备id", this.deviceId);
            setTimeout(() => {
              this.getBLEDeviceServices(this.deviceId);
            }, 1e3);
          },
          fail: (err) => {
            uni.showToast({
              title: "建立连接失败",
              icon: "none"
            });
            return;
          }
        });
        this.stopBluetoothDevicesDiscovery();
      },
      //获取蓝牙设备所有服务（走到这一步证明已经找到服务）
      getBLEDeviceServices(deviceId) {
        uni.getBLEDeviceServices({
          deviceId,
          success: (res) => {
            this.getBLEDeviceCharacteristics(deviceId, this.serviceId);
          },
          fail: (err) => {
            uni.showToast({
              title: "获取服务失败",
              icon: "none"
            });
            return;
          }
        });
      },
      //如果一个蓝牙设备需要进行数据的写入以及数据传输，就必须具有某些特征值，所以通过上面步骤获取的id可以查看当前蓝牙设备的特征值
      getBLEDeviceCharacteristics(deviceId, serviceId) {
        uni.getBLEDeviceCharacteristics({
          //设备id与服务id必须要给,特征值id先不用获取，直接写死
          deviceId,
          serviceId,
          success: (res) => {
            if (res.characteristics[0].properties.read) {
              formatAppLog("log", "at pages/index/index.vue:473", "该特征值可读");
              uni.readBLECharacteristicValue({
                deviceId,
                serviceId,
                characteristicId: this.characteristicId
              });
            }
            if (res.characteristics[0].properties.write) {
              formatAppLog("log", "at pages/index/index.vue:482", "该特征值可写");
              this.canWrite = true;
              this.writeBLECharacteristicValue();
            }
            if (res.characteristics[0].properties.notify || res.characteristics[2].properties.indicate) {
              uni.notifyBLECharacteristicValueChange({
                deviceId,
                serviceId,
                characteristicId: this.characteristicId,
                state: true,
                //是否启用notify通知
                success: (res2) => {
                  formatAppLog("log", "at pages/index/index.vue:495", "通知启用(notifyBLECharacteristicValueChange)", res2);
                }
              });
            }
          },
          fail(res) {
            formatAppLog("error", "at pages/index/index.vue:501", "获取蓝牙设备特征值失败(getBLEDeviceCharacteristics)", res);
          }
        });
        uni.onBLECharacteristicValueChange((characteristic) => {
          formatAppLog("log", "at pages/index/index.vue:507", "触发监听(onBLECharacteristicValueChange)");
          formatAppLog("log", "at pages/index/index.vue:508", "手机接收的数据：", this.ab2hex(characteristic.value));
          this.myDataMeasure.push(this.ab2hex(characteristic.value));
          const idx = this.inArray(this.chs, "uuid", characteristic.characteristicId);
          if (idx === -1) {
            this.chs.push({
              uuid: characteristic.characteristicId,
              value: this.ab2hex(characteristic.value)
            });
          } else {
            this.chs[idx] = {
              uuid: characteristic.characteristicId,
              value: this.ab2hex(characteristic.value)
            };
          }
          formatAppLog("log", "at pages/index/index.vue:526", this.myDataMeasure);
        });
      },
      //向蓝牙设备发送一个数据（期间要进行数据转换）
      writeBLECharacteristicValue() {
        formatAppLog("log", "at pages/index/index.vue:531", "我正在发送数据【手机向蓝牙模块】");
        let buffer = new ArrayBuffer(1);
        let dataView = new DataView(buffer);
        dataView.setUint8(0, this.sendData);
        uni.writeBLECharacteristicValue({
          deviceId: this.deviceId,
          serviceId: this.serviceId,
          characteristicId: this.characteristicId,
          value: buffer,
          complete: () => {
            formatAppLog("log", "at pages/index/index.vue:545", "十六进制", this.ab2hex(buffer));
            let sixNumber = this.ab2hex(buffer);
            formatAppLog("log", "at pages/index/index.vue:547", "十进制", this.hex2int(sixNumber));
          }
        });
      },
      //停止蓝牙设备搜寻
      stopBluetoothDevicesDiscovery() {
        uni.stopBluetoothDevicesDiscovery();
        formatAppLog("log", "at pages/index/index.vue:555", "已停止蓝牙扫描(closeBluetoothAdapter)");
        uni.showToast({
          title: "蓝牙搜索已停止",
          icon: "success"
        });
      },
      //关闭搜索
      closeBlueTooth() {
        uni.stopBluetoothDevicesDiscovery();
        formatAppLog("log", "at pages/index/index.vue:564", "已关闭蓝牙搜索(closeBlueTooth)");
      },
      //关闭蓝牙适配器
      closeBluetoothAdapter() {
        uni.closeBluetoothAdapter();
        this._discoveryStarted = false;
        formatAppLog("log", "at pages/index/index.vue:570", "已关闭蓝牙适配器(closeBluetoothAdapter)");
        this.devices = [];
        uni.showToast({
          title: "适配器关闭成功",
          icon: "success"
        });
      },
      //关闭蓝牙连接
      closeBLEConnection() {
        uni.closeBLEConnection({
          deviceId: this.deviceId
        });
        this.connected = false;
        this.chs = [];
        this.canWrite = false;
        formatAppLog("log", "at pages/index/index.vue:586", "已关闭蓝牙连接(closeBLEConnection)");
        uni.showToast({
          title: "蓝牙连接已断开",
          icon: "success"
        });
      }
    }
  };
  function _sfc_render$1(_ctx, _cache, $props, $setup, $data, $options) {
    const _component_u_button = vue.resolveComponent("u-button");
    const _component_u_col = vue.resolveComponent("u-col");
    const _component_u_row = vue.resolveComponent("u-row");
    const _component_u_input = vue.resolveComponent("u-input");
    return vue.openBlock(), vue.createElementBlock("view", { class: "content" }, [
      vue.createCommentVNode(" 实现蓝牙功能区 "),
      vue.createVNode(_component_u_row, null, {
        default: vue.withCtx(() => [
          vue.createVNode(_component_u_col, { span: "12" }, {
            default: vue.withCtx(() => [
              vue.createVNode(_component_u_button, {
                type: "success",
                onClick: $options.openBluetoothAdapter
              }, {
                default: vue.withCtx(() => [
                  vue.createTextVNode("开始扫描(初始化蓝牙)")
                ]),
                _: 1
                /* STABLE */
              }, 8, ["onClick"]),
              vue.createElementVNode("br")
            ]),
            _: 1
            /* STABLE */
          })
        ]),
        _: 1
        /* STABLE */
      }),
      vue.createVNode(_component_u_row, null, {
        default: vue.withCtx(() => [
          vue.createVNode(_component_u_col, { span: "12" }, {
            default: vue.withCtx(() => [
              vue.createVNode(_component_u_button, {
                type: "warning",
                onClick: $options.stopBluetoothDevicesDiscovery
              }, {
                default: vue.withCtx(() => [
                  vue.createTextVNode("停止扫描(停止蓝牙搜索)")
                ]),
                _: 1
                /* STABLE */
              }, 8, ["onClick"]),
              vue.createElementVNode("br")
            ]),
            _: 1
            /* STABLE */
          })
        ]),
        _: 1
        /* STABLE */
      }),
      vue.createVNode(_component_u_row, null, {
        default: vue.withCtx(() => [
          vue.createVNode(_component_u_col, { span: "12" }, {
            default: vue.withCtx(() => [
              vue.createVNode(_component_u_button, {
                type: "error",
                onClick: $options.closeBluetoothAdapter
              }, {
                default: vue.withCtx(() => [
                  vue.createTextVNode("结束流程(关闭蓝牙适配器)")
                ]),
                _: 1
                /* STABLE */
              }, 8, ["onClick"]),
              vue.createElementVNode("br")
            ]),
            _: 1
            /* STABLE */
          })
        ]),
        _: 1
        /* STABLE */
      }),
      vue.createVNode(_component_u_row, null, {
        default: vue.withCtx(() => [
          vue.createVNode(_component_u_col, { span: "12" }, {
            default: vue.withCtx(() => [
              vue.createVNode(_component_u_button, {
                type: "primary",
                disabled: $data.myTemplateKey.length == 0,
                onClick: $options.sendStorage
              }, {
                default: vue.withCtx(() => [
                  vue.createTextVNode("发送数据")
                ]),
                _: 1
                /* STABLE */
              }, 8, ["disabled", "onClick"]),
              vue.createElementVNode("br")
            ]),
            _: 1
            /* STABLE */
          })
        ]),
        _: 1
        /* STABLE */
      }),
      vue.createCommentVNode(" 搜索蓝牙设备的对话框 "),
      !$data.connected ? (vue.openBlock(), vue.createElementBlock(
        "view",
        {
          key: 0,
          class: "devices_summary"
        },
        "已发现 " + vue.toDisplayString($data.devices.length) + " 个外围设备：",
        1
        /* TEXT */
      )) : vue.createCommentVNode("v-if", true),
      !$data.connected ? (vue.openBlock(), vue.createElementBlock("scroll-view", {
        key: 1,
        class: "device_list",
        "scroll-y": "",
        "scroll-with-animation": ""
      }, [
        (vue.openBlock(true), vue.createElementBlock(
          vue.Fragment,
          null,
          vue.renderList($data.devices, (item, index) => {
            return vue.openBlock(), vue.createElementBlock("view", {
              key: index,
              "data-device-id": item.deviceId,
              "data-name": item.name || item.localName,
              onClick: _cache[0] || (_cache[0] = ($event) => $options.createBLEConnection($event)),
              class: "device_item",
              "hover-class": "device_item_hover"
            }, [
              vue.createElementVNode(
                "view",
                { style: { "font-size": "16px", "color": "#333" } },
                vue.toDisplayString(item.name),
                1
                /* TEXT */
              ),
              vue.createElementVNode(
                "view",
                { style: { "font-size": "10px" } },
                "信号强度: " + vue.toDisplayString(item.RSSI) + "dBm (" + vue.toDisplayString($options.myMax(0, item.RSSI + 100)) + "%) ",
                1
                /* TEXT */
              ),
              vue.createElementVNode(
                "view",
                { style: { "font-size": "10px" } },
                "UUID: " + vue.toDisplayString(item.deviceId),
                1
                /* TEXT */
              ),
              vue.createElementVNode(
                "view",
                { style: { "font-size": "10px" } },
                "Service数量: " + vue.toDisplayString($options.myLen(item.advertisServiceUUIDs)),
                1
                /* TEXT */
              )
            ], 8, ["data-device-id", "data-name"]);
          }),
          128
          /* KEYED_FRAGMENT */
        ))
      ])) : vue.createCommentVNode("v-if", true),
      vue.createCommentVNode(" 接收蓝牙模块传入数据的对话框 "),
      $data.connected ? (vue.openBlock(), vue.createElementBlock("view", {
        key: 2,
        class: "devices_summary"
      }, [
        vue.createVNode(_component_u_row, null, {
          default: vue.withCtx(() => [
            vue.createVNode(_component_u_col, { span: "3" }, {
              default: vue.withCtx(() => [
                vue.createElementVNode("view", null, [
                  vue.createVNode(_component_u_button, { onClick: $options.saveGroup }, {
                    default: vue.withCtx(() => [
                      vue.createTextVNode("保存本组")
                    ]),
                    _: 1
                    /* STABLE */
                  }, 8, ["onClick"])
                ])
              ]),
              _: 1
              /* STABLE */
            }),
            vue.createVNode(_component_u_col, { span: "3" }, {
              default: vue.withCtx(() => [
                vue.createElementVNode("view", null, [
                  vue.createVNode(_component_u_button, { onClick: $options.saveStorage }, {
                    default: vue.withCtx(() => [
                      vue.createTextVNode("保存本次")
                    ]),
                    _: 1
                    /* STABLE */
                  }, 8, ["onClick"])
                ])
              ]),
              _: 1
              /* STABLE */
            }),
            vue.createVNode(_component_u_col, { span: "3" }, {
              default: vue.withCtx(() => [
                vue.createElementVNode("view", null, [
                  vue.createVNode(_component_u_button, { onClick: $options.clearLocalStorage }, {
                    default: vue.withCtx(() => [
                      vue.createTextVNode("清空本地")
                    ]),
                    _: 1
                    /* STABLE */
                  }, 8, ["onClick"])
                ])
              ]),
              _: 1
              /* STABLE */
            }),
            vue.createVNode(_component_u_col, { span: "3" }, {
              default: vue.withCtx(() => [
                vue.createElementVNode("view", null, [
                  vue.createVNode(_component_u_button, {
                    onClick: _cache[1] || (_cache[1] = ($event) => $data.myDataMeasure = [])
                  }, {
                    default: vue.withCtx(() => [
                      vue.createTextVNode("清空接收栏")
                    ]),
                    _: 1
                    /* STABLE */
                  })
                ])
              ]),
              _: 1
              /* STABLE */
            })
          ]),
          _: 1
          /* STABLE */
        }),
        vue.createVNode(_component_u_row, null, {
          default: vue.withCtx(() => [
            vue.createVNode(_component_u_col, { span: "3" }, {
              default: vue.withCtx(() => [
                vue.createElementVNode("view", null, "接收数据:")
              ]),
              _: 1
              /* STABLE */
            })
          ]),
          _: 1
          /* STABLE */
        })
      ])) : vue.createCommentVNode("v-if", true),
      $data.connected ? (vue.openBlock(), vue.createElementBlock("scroll-view", {
        key: 3,
        class: "device_list",
        "scroll-y": "",
        "scroll-with-animation": ""
      }, [
        (vue.openBlock(true), vue.createElementBlock(
          vue.Fragment,
          null,
          vue.renderList($data.myDataMeasure, (item, index) => {
            return vue.openBlock(), vue.createElementBlock(
              "view",
              { key: index },
              vue.toDisplayString(item),
              1
              /* TEXT */
            );
          }),
          128
          /* KEYED_FRAGMENT */
        ))
      ])) : vue.createCommentVNode("v-if", true),
      vue.createCommentVNode(" 显示连接对话框 "),
      $data.connected ? (vue.openBlock(), vue.createElementBlock("view", {
        key: 4,
        class: "connected_info"
      }, [
        vue.createElementVNode("view", null, [
          vue.createElementVNode(
            "text",
            null,
            "已连接到 " + vue.toDisplayString($data.deviceName),
            1
            /* TEXT */
          ),
          vue.createElementVNode("view", { class: "operation" }, [
            vue.createElementVNode(
              "button",
              {
                size: "mini",
                onClick: _cache[2] || (_cache[2] = (...args) => $options.hideShowInfo && $options.hideShowInfo(...args))
              },
              vue.toDisplayString($data.infoShowHideButton),
              1
              /* TEXT */
            ),
            $data.canWrite ? (vue.openBlock(), vue.createElementBlock("button", {
              key: 0,
              size: "mini",
              onClick: _cache[3] || (_cache[3] = (...args) => $options.writeBLECharacteristicValue && $options.writeBLECharacteristicValue(...args))
            }, "写数据")) : vue.createCommentVNode("v-if", true),
            vue.createElementVNode("button", {
              size: "mini",
              onClick: _cache[4] || (_cache[4] = (...args) => $options.closeBLEConnection && $options.closeBLEConnection(...args))
            }, "断开连接")
          ])
        ]),
        (vue.openBlock(true), vue.createElementBlock(
          vue.Fragment,
          null,
          vue.renderList($data.chs, (item, index) => {
            return vue.withDirectives((vue.openBlock(), vue.createElementBlock("view", {
              key: index,
              style: { "font-size": "12px", "margin-top": "10px" }
            }, [
              vue.createElementVNode(
                "view",
                null,
                "特性UUID: " + vue.toDisplayString(item.uuid),
                1
                /* TEXT */
              ),
              vue.createElementVNode(
                "view",
                null,
                "接收特性值(当前): " + vue.toDisplayString(item.value),
                1
                /* TEXT */
              )
            ])), [
              [vue.vShow, $data.infoShowHide]
            ]);
          }),
          128
          /* KEYED_FRAGMENT */
        )),
        $data.canWrite ? vue.withDirectives((vue.openBlock(), vue.createElementBlock(
          "view",
          { key: 0 },
          [
            vue.createVNode(_component_u_input, {
              type: "number",
              modelValue: $data.sendData,
              "onUpdate:modelValue": _cache[5] || (_cache[5] = ($event) => $data.sendData = $event)
            }, null, 8, ["modelValue"])
          ],
          512
          /* NEED_PATCH */
        )), [
          [vue.vShow, $data.infoShowHide]
        ]) : vue.createCommentVNode("v-if", true)
      ])) : vue.createCommentVNode("v-if", true)
    ]);
  }
  const PagesIndexIndex = /* @__PURE__ */ _export_sfc(_sfc_main$2, [["render", _sfc_render$1], ["__scopeId", "data-v-1cf27b2a"], ["__file", "/Users/YellowWenJie/Desktop/stm32_study/web/pages/index/index.vue"]]);
  const _sfc_main$1 = {};
  function _sfc_render(_ctx, _cache, $props, $setup, $data, $options) {
    return vue.openBlock(), vue.createElementBlock("view", { class: "content" });
  }
  const PagesSettingsIndex = /* @__PURE__ */ _export_sfc(_sfc_main$1, [["render", _sfc_render], ["__file", "/Users/YellowWenJie/Desktop/stm32_study/web/pages/settings/index.vue"]]);
  __definePage("pages/index/index", PagesIndexIndex);
  __definePage("pages/settings/index", PagesSettingsIndex);
  const _sfc_main = {
    onLaunch: function() {
      formatAppLog("log", "at App.vue:4", "App Launch");
    },
    onShow: function() {
      formatAppLog("log", "at App.vue:7", "App Show");
    },
    onHide: function() {
      formatAppLog("log", "at App.vue:10", "App Hide");
    }
  };
  const App = /* @__PURE__ */ _export_sfc(_sfc_main, [["__file", "/Users/YellowWenJie/Desktop/stm32_study/web/App.vue"]]);
  function createApp() {
    const app = vue.createVueApp(App);
    return {
      app
    };
  }
  const { app: __app__, Vuex: __Vuex__, Pinia: __Pinia__ } = createApp();
  uni.Vuex = __Vuex__;
  uni.Pinia = __Pinia__;
  __app__.provide("__globalStyles", __uniConfig.styles);
  __app__._component.mpType = "app";
  __app__._component.render = () => {
  };
  __app__.mount("#app");
})(Vue);
