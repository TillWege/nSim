// The project function defines how your document looks.
// It takes your content and some metadata and formats it.
// Go ahead and customize it to your liking!
#let project(title: "", subtitle: "", authors: (), date: none, body) = {
  // Set the document's basic properties.
  set document(author: authors, title: title)
  
  set text(font: "New Computer Modern Math", size: 11pt, lang: "de", spacing: 1.5pt)
  set heading(numbering: "1.1")

  // Title row.
  align(center)[
    #block(text(weight: 500, 1.75em, spacing: 5pt, title))
    #v(1em, weak: true)
    #date
  ]

  align(center)[
    #block(text(weight: 400, 1.0em, subtitle))
    #v(1em, weak: true)
  ]

  let currentAuthorIndex = 0

  // Author information.
  pad(
    top: 0.5em,
    bottom: 0.5em,
    x: 2em,
    grid(
      columns: (1fr,) * calc.min(3, authors.len()),
      gutter: 1em,
      ..authors.slice(0, currentAuthorIndex).map(author => align(center, (author))),
      ..authors.slice(currentAuthorIndex, currentAuthorIndex + 1).map(author => align(center, strong(author))),
      ..authors.slice(currentAuthorIndex + 1, authors.len()).map(author => align(center, (author))),
    ),
  )

  // Main body.
  set par(justify: true)

  
  show raw.where(block: true): block.with(
     fill: luma(250),
     inset: 2pt,
     radius: 4pt,
     width: 100%,
  )


  body
}