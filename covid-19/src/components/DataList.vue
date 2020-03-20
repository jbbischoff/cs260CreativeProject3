<template>
  <div class="data-list">
    <div class="data-group" v-for="(data, index) in dataSet" v-bind:key="index">
      <div class="data-group-header data-group-element">
        <div class="data-group-title">
          <!-- <i v-on:click="hideVerbose(index)" class="fas" v-bind:class="{'fa-angle-down': showVerboseData['index'], 'fa-angle-right': !showVerboseData['index']}"></i> -->
          <i v-if="showVerboseData[index]" v-on:click="hideVerbose(index)" class="fas fa-angle-down"></i>
          <i v-else v-on:click="showVerbose(index)" class="fas fa-angle-right"></i>
          <h1>{{data.id}}</h1>
        </div>
        <div class="data-info">
          <div class="header-info">
            <p class="header-description">Confirmed</p>
            <p>{{data.totalConfirmed}}</p>
          </div>
          <div class="header-info">
            <p class="header-description">Deaths</p>
            <p>{{data.totalDeaths}}</p>
          </div>
          <div class="header-info">
            <p class="header-description">Recovered</p>
            <p>{{data.totalRecovered}}</p>
          </div>
        </div>
      </div>
      <div v-if="showVerboseData[index]" class="data-group-verbose">
        <div class="data-subgroup-element data-group-element" v-for="(subdata, date) in data.verbose" v-bind:key="date">
          <div class="data-group-title">
            <h2>{{date}}</h2>
          </div>
          <div class="data-info">
            <p>{{subdata.confirmed}}</p>
            <p>{{subdata.deaths}}</p>
            <p>{{subdata.recovered}}</p>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'DataList',
  props: {
    dataSet: Array,
    dataSetShowVerbose: Array,
  },
  methods: {
    showVerbose(index) {
      //DataList.set(this.dataSet[id], "showVerbose", true);
      this.dataSetShowVerbose.splice(index, 1, true);
    },
    hideVerbose(index) {
      this.dataSetShowVerbose.splice(index, 1, false);
      //this.dataSet[id].showVerbose = false;
    }
  },
  computed: {
    showVerboseData() {
      return this.dataSetShowVerbose;
    }
  }
}
</script>

<style scoped>
.data-list {
  width: 100%;
  display: flex;
  flex-direction: column;
  justify-content: flex-start;
  align-items: center;
}

.data-group {
  width: 100%;
  max-width: 48rem;
  margin: 0.2rem;
  padding: 0 5px;
  background-color: #eee;
  color: #222;
  display: flex;
  flex-direction: column;
  justify-content: flex-start;
}

.data-group-verbose {
  width: 100%;
  border-top: 1px solid #888;
  display: flex;
  flex-direction: column;
  justify-content: flex-start;
}

.data-group-element {
  width: 100%;
  margin: 5px;
  display: flex;
  justify-content: space-between;
  align-items: center;
}

.data-group-title {
  display: flex;
  justify-content: flex-start;
  align-items: center;
}

.data-group-title i {
  margin: 0.4rem;
}

.data-group-title h1 {
  font-size: 1.5rem;
}

.data-group-title h2 {
  font-size: 1.2rem;
}

.data-subgroup-element .data-group-title {
  margin-left: 2rem;
}

.data-info {
  display: flex;
  justify-content: flex-end;
}

.data-info * {
  width: 6rem;
  margin: 0 0.8rem;
  text-align: center;
}

.header-info p {
  width: 100%;
  margin: 0;
}

.header-description {
  font-weight: bold;
}
</style>
