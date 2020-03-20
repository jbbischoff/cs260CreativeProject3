import Vue from 'vue'
import VueRouter from 'vue-router'
import Home from '../views/Home.vue'
import State from '../views/State.vue'
import City from '../views/City.vue'

Vue.use(VueRouter)

const routes = [
  {
    path: '/',
    name: 'Home',
    component: Home
  },
  {
    path: '/state',
    name: 'State',
    component: State
  },
  {
    path: '/city',
    name: 'City',
    component: City
  },
]

const router = new VueRouter({
  mode: 'history',
  base: process.env.BASE_URL,
  routes
})

export default router
