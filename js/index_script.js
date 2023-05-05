const socket = new WebSocket("ws://localhost:5000");

socket.addEventListener("open", () => {
  console.log("Socket connection established");

  // Request movie titles from server
  socket.send("get_movie_titles");
});

socket.addEventListener("message", (event) => {
  console.log("Received movie titles from server");
  const movieList = document.querySelector(".movie-list");

  // Remove existing movie elements from list
  while (movieList.firstChild) {
    movieList.removeChild(movieList.firstChild);
  }

  // Parse movie titles from server response
  const movieTitles = event.data.split("\n");

  // Create new movie element for each title
  movieTitles.forEach((title) => {
    if (title.trim() !== "") { 
      const movie = document.createElement("div");
      movie.classList.add("movie");
      movie.innerHTML = `
        <img
          src="../media/${encodeURIComponent(title)}.jpg"
          alt="${title} Poster"
        />
        <h2>${title}</h2>
        <p>Description of ${title} goes here.</p>
        <a href="booking.html?movie=${encodeURIComponent(
          title
        )}" class="btn">Book Now</a>
      `;
      movieList.appendChild(movie);
    }
  });
});

socket.addEventListener("close", () => {
  console.log("Socket connection closed");
});

socket.addEventListener("error", (error) => {
  console.error("Socket error:", error);
});
