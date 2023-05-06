// Script para la página index.html

/* Se crea una instancia de XMLHttpRequest para hacer una petición al servidor y obtener los datos de las películas */
const xhr = new XMLHttpRequest();
xhr.open("GET", "http://localhost:5000/movies");

xhr.onload = () => {
  console.log("Información de películas obtenida");
  const movieList = document.querySelector(".movie-list");

  /* Se eliminan los elementos de la lista de películas */
  while (movieList.firstChild) {
    movieList.removeChild(movieList.firstChild);
  }

  /* Se obtiene la información de las películas.*/
  const movieData = JSON.parse(xhr.responseText);

  /* Se crea un elemento HTML para cada película y se añade a la lista de películas */
  movieData.forEach((data) => {
    const { title, description, showtime } = data;

    if (title.trim() !== "") {
      const movie = document.createElement("div");
      movie.classList.add("movie");
      movie.innerHTML = `
        <img src="../media/${encodeURIComponent(title)}.jpg" alt="${title} Poster"/>
        <div class="movie-details">
          <h2 class="movie-title">${title}</h2>
          <p class="movie-desc">${description.substr(0, 100)}...</p>
          <a href="booking.html?movie=${encodeURIComponent(title)}" class="movie-btn">Book Now</a>
        </div>
        `;
      movieList.appendChild(movie);
    }
  });
};

/* Se muestra un mensaje de error en la consola si la petición falla */
xhr.onerror = (error) => {
  console.error("XHR error:", error);
};

/* Se envía la petición al servidor */
xhr.send();