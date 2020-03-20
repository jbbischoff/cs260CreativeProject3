<template>
  <div class="home">
    <div class="wrapper">
      <div class="search">
        <form class="pure-form">
          <i class="fas fa-search"></i><input v-model="searchText" />
        </form>
      </div>
    </div>
    <DataList :dataSet="displayData" :dataSetShowVerbose="displayDataShowVerbose" />
  </div>
</template>

<script>
import DataList from "../components/DataList.vue"

export default {
  name: 'Home',
  components: {
    DataList
  },
  data() {
    return {
      searchText: '',
    }
  },
  computed: {
    dataSet() {
      return this.$root.$data.dataSet.reduce((result, element) => {
        if (element.countryOrRegion == "US") {
          if (Object.prototype.hasOwnProperty.call(result, element.provinceOrState)) {
            result[element.provinceOrState].totalConfirmed += element.confirmed;
            result[element.provinceOrState].totalDeaths += element.deaths;
            result[element.provinceOrState].totalRecovered += element.recovered;

            if (Object.prototype.hasOwnProperty.call(result[element.provinceOrState].verbose, element.dateReported)) {
              result[element.provinceOrState].verbose[element.dateReported].confirmed += element.confirmed;
              result[element.provinceOrState].verbose[element.dateReported].deaths += element.deaths;
              result[element.provinceOrState].verbose[element.dateReported].recovered += element.recovered;
            }
            else {
              result[element.provinceOrState].verbose[element.dateReported] = {
                date: element.dateReported,
                confirmed: element.confirmed,
                deaths: element.deaths,
                recovered: element.recovered
              };
            }
          }
          else {
            result[element.provinceOrState] = {
              id: element.provinceOrState,
              totalConfirmed: element.confirmed,
              totalDeaths: element.deaths,
              totalRecovered: element.recovered,
              verbose: {}
            };
            result[element.provinceOrState].verbose[element.dateReported] = {
              confirmed: element.confirmed,
              deaths: element.deaths,
              recovered: element.recovered
            };
          }
        }

        return result;
      }, {});
    },
    displayData() {
      //key => key.toLowerCase().search(this.searchText.toLowerCase()) >= 0
      function containsSubstring(value, substring) {
        return (value.toLowerCase().search(substring.toLowerCase()) >= 0);
      }

      return Object.keys(this.dataSet).sort()
          .filter(key => containsSubstring(key, this.searchText))
          .reduce((result, key) => {
              result.push(this.dataSet[key]);
              return result;
            }, []);
    },
    displayDataShowVerbose() {
      return Object.keys(this.displayData).reduce((result, key) => {result.push(key !== key); return result;}, []);
    }
  }
}
</script>

<style scoped>
.home {
  margin: 1rem 3rem;
}

.wrapper {
  display: flex;
  align-items: center;
  justify-content: center;
}

.search {
  margin: 0.5rem;
  margin-bottom: 1rem;
  border: 1px solid #ccc;
  border-radius: 4px;
  width: 50%;
  max-width: 30rem;
}

form {
  display: table;
  width: 100%;
}

i {
  display: table-cell;
  padding-left: 10px;
  width: 1px;
}

input {
  display: table-cell;
  font-size: 20px;
  border: none !important;
  box-shadow: none !important;
  width: 100%;
  height: 2rem;
}
</style>
