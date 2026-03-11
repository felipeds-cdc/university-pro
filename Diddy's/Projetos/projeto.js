AOS.init();
document.querySelectorAll('.btn-filtro').forEach(button => {
  button.addEventListener('click', () => {
    const category = button.getAttribute('data-category');
    const projetos = document.querySelectorAll('.card-projeto');

    projetos.forEach(projeto => {
      // Mostrar todos os projetos se "all" for selecionado
      if (category === 'all' || projeto.getAttribute('data-category') === category) {
        projeto.style.display = 'block';
      } else {
        projeto.style.display = 'none';
      }
    });
  });
});
