<template>
  <div class="state">
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
  name: 'State',
  components: {
    DataList
  },
  data() {
    return {
      searchText: '',
    }
  },
  computed: {
    stateNameToCode() {
      return this.$root.$data.states.reduce((result, element) => {result[element.state] = element.code; return result;}, {});
    },
    dataSet() {
      return this.$root.$data.dataSet.reduce((result, element) => {
        if (element.countryOrRegion === "US") {
          let stateCode = '';
          if (element.provinceOrState.search(',') >= 0) {
            stateCode = element.provinceOrState.substring(element.provinceOrState.search(',') + 2);

            if (stateCode.length > 2) {
              stateCode = stateCode.replace(/\./g, '');

              if (stateCode.length > 2) {
                stateCode = stateCode.substring(0, 2);
              }
            }
          }
          else {
            if (element.provinceOrState in this.stateNameToCode) {
              stateCode = this.stateNameToCode[element.provinceOrState];
            }
            else {
              if (element.provinceOrState == "Chicago") {
                stateCode = 'IL';
              }
              else if (element.provinceOrState.search("Princess") >= 0) {
                stateCode = "~Unknown (Curise Ship)";
              }
              else {
                stateCode = '~ ' + element.provinceOrState;
                stateCode = stateCode.replace("United States ", '');
              }
            }
          }

          if (Object.prototype.hasOwnProperty.call(result, stateCode)) {
            result[stateCode].totalConfirmed += element.confirmed;
            result[stateCode].totalDeaths += element.deaths;
            result[stateCode].totalRecovered += element.recovered;

            if (Object.prototype.hasOwnProperty.call(result[stateCode].verbose, element.dateReported)) {
              result[stateCode].verbose[element.dateReported].confirmed += element.confirmed;
              result[stateCode].verbose[element.dateReported].deaths += element.deaths;
              result[stateCode].verbose[element.dateReported].recovered += element.recovered;
            }
            else {
              result[stateCode].verbose[element.dateReported] = {
                date: element.dateReported,
                confirmed: element.confirmed,
                deaths: element.deaths,
                recovered: element.recovered
              };
            }
          }
          else {
            result[stateCode] = {
              id: stateCode,
              totalConfirmed: element.confirmed,
              totalDeaths: element.deaths,
              totalRecovered: element.recovered,
              verbose: {}
            };
            result[stateCode].verbose[element.dateReported] = {
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
.state {
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
