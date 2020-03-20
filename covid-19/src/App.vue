<template>
  <div id="app">
    <div id="menu">
      <div id="brand">
        <router-link to="/">
          <p class="brand-item">COVID-19 Data</p>
        </router-link>
      </div>
      <div id="nav">
        <router-link to="/">
          <div class="menu-item browse">
            <p>Country</p>
          </div>
        </router-link>
        <router-link to="/state">
          <div class="menu-item browse">
            <p>State</p>
          </div>
        </router-link>
        <router-link to="/city">
          <div class="menu-item browse">
            <p>City</p>
          </div>
        </router-link>
      </div>
    </div>
    <router-view/>
    <div id="footer">
      <a href="https://github.com/jbbischoff/cs260CreativeProject3.git">GitHub</a>
      <a href="https://github.com/CSSEGISandData/COVID-19.git">Source</a>
    </div>
  </div>
</template>

<style>
* {
  box-sizing: border-box;
  margin: 0;
  padding: 0;
}

#menu {
  display: grid;
  grid-template-columns: 1fr 1fr 1fr;
  grid-column-gap: 5px;
  grid-template-areas: "none brand side";
  padding: 2rem;
  background-color: #eee;
}

#menu a {
  color: #222;
  text-decoration: none;
}

#menu a:focus, #menu a:hover {
  color: #666;
}

#brand {
  grid-area: brand;
  display: flex;
  justify-content: center;
  align-items: center;
  font-size: 1.5rem;
}

#nav {
  grid-area: side;
  display: flex;
  justify-content: right;
  align-items: center;
}

.menu-item {
  display: flex;
  flex-direction: column;
}

.browse {
  margin-right: 1.5rem;
}

#footer {
  margin-top: 1rem;
  padding: 1rem;
  background: #eee;
  display: flex;
  justify-content: center;
  align-items: center;
}

#footer a {
  margin: 0.5rem 1rem;
  text-decoration: none;
  color: #000;
}

#footer a:hover, #footer:focus {
  color: #444;
}
</style>
