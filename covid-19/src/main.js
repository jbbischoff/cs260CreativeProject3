import Vue from 'vue'
import App from './App.vue'
import router from './router'
import stateData from './stateData.js'
import covidData from './covidData.js'

let data = {
  dataSet: covidData,
  states: stateData
}

Vue.config.productionTip = false

new Vue({
  router,
  data,
  render: h => h(App)
}).$mount('#app')
