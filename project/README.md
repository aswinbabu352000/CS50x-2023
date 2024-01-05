# ResMkr
### Video Demo: https://youtu.be/C37IvkKT978
### Description: A dynamic resume builder web app created using the python web framework Flask, HTML, CSS, JavaScript and SQLite.

## Features
- Users can see their resume shaping-up in real time.
- They can download their resume as a PDF.
- They can save their details, allowing easy generation of new resume with minor or major changes on the fly.
- Users can upload thier images.
- They can edit the different paragraphs in thier resume using a rich text editor.

## Frameworks and Libraries

- CKEditor, for integrating a a rich text edior. [View Documentation](https://ckeditor.com/old/sites/default/files/uploads/CKEditor%204%20Documentation.pdf)
- Lux theme by Bootswatch, to create the asethetics of the app. [View Theme](https://bootswatch.com/lux/)

## Explaining the inner Workings
### Resume
- The 'Resume' in the editor is created in HTML using Bootstrap classes.
- It has a fixed width and height of an A4 size paper. This makes the app current not-smartphone-friendly.

    CSS Styling
    ```css
        .resume-layout {
            width: 21cm ;
            height: 29.7cm;
            max-width: 100%;
            max-height: 100%;
            box-sizing: border-box;
        }
    ```
- Currently only the color theme of the resume can be changed. It done using different colored buttons corresponding to the color themes and a JS function.
Which will change the background-color of three different divs inside the resume-layout.

    ```css
        #header {
            background-color: rgb(66, 72, 116);
            color: white ;
            height:  6cm;
            max-height: 100%;
        }
        #sidebar {
            background-color: rgb(220, 214, 247);
            color: black;
            height: 23.7cm;
            max-height: 100%;
        }
        #mainbody {
            background-color: rgb(244, 238, 255);
            color: black;
            height: 23.7cm;
            max-height: 100%;
        }
    ```

### Editor
- The editor is essentially a form consisting of different input types and a submit button (named 'Save').

#### Image Upload
- Image upload is done using the "post" method of a seperate form which saves the images the use uploads into folder with the file names generated during upload.

    ```python
    if request.method == "POST":
        uploaded_image = request.files['file']
        if uploaded_image.filename != '':
            photoname = 'photo'+ str(session["user_id"])
            file_path = os.path.join("/workspaces/132731110/finalproject/resumeapp/static/photos", photoname)
            uploaded_image.save(file_path)
    ```

#### Normal texts
- Normal texts like Name, Title etc which do no need formating is taken directly from the value of the text inputs through 'request.form.get()'.

    Html
    ```html
    <input autocomplete="off" name="name" type="text" class="form-control" id="nameinput" oninput="updateOutput('nameinput', 'name')">
    ```
    Flask
    ```python
    name = request.form.get("name")
    ```

#### Paragraph Inputs
- Paragraph Inputs for Profile and Job Discription are created using CKEditor, which converts a textarea into a rich text editor.

    Html for creating a textarea
    ```html
    <textarea autocomplete="off" id="jobDiscription1input" name="jobDiscription1input"></textarea>
    ```
    JS for converting into Rich Text Editor.
    ```js
    var jobDiscription1CkEditor = CKEDITOR.instances['jobDiscription1input'];
    ```

#### Theme selection
- Theme is also got useing the post function, but the theme selectors are buttons, inorder to do we have a hidden form that changes the value to the id of the button which is pressed.

- The JS code is part the 'changeTheme' function which changes the theme of the resume according to the different theme-buttons that are clicked.

    Html
    ```html
    <input autocomplete="off" hidden id="selectedTheme" value="blue" name="selectedTheme">
    ```
    Js
    ```js
    document.getElementById('selectedTheme').value = color;
    ```
    Flask
    ```python
    theme = request.form.get('selectedTheme')
    ```
### Database
- The app used a Sqlite database to store data. As seen above a simple form submission via post get us all the datails the user has entered in editor.
- The database contains two different tables. 1. Users, 2. Details.
- User is used fot storing the user names and hashed passwords.
- Details is used to store all the details from the input forms of our editor.
- We then store each value to the correspoding table of our database.

### Dynamic Editing
- Using simple Javascript function which is triggered on 'oninput' all our input gets shown in real time on our resume.
- Lets see en example for the 'title' input.

    Html of input field
    ```html
        <input autocomplete="off" name="job_title" type="text" class="form-control" id="titleinput" oninput="updateOutput('titleinput', 'title')">
    ```
    Html of correspoding output text
    ```html
        <h4 id="title" class="text-start mt-0">Title</h4>
    ```
    Lets see how the function works
    ```JS
        function updateOutput(input, show) {
            var xInput = document.getElementById(input).value;
            var x = document.getElementById(show);
            x.textContent = xInput;
        }
    ```

### Downloading as PDF
- On clicking the 'Download as PDF' button, we have a setup its on click as:
    ```JS
    window.print()
    ```
- This essentially works for the whole page. So we haves used '@media print' styles in our CSS in way that only the resume gets printed while eveything else is hidden.
    ```CSS
        @media print {
        body {
            visibility: hidden;
        }
        .resume-show {
            visibility: visible;
            display: block;
            position: absolute;
            left: 0;
            top: 0;
            padding: 20;
            margin: 0;
            height: 29.7cm;
            width: 21cm ;
            overflow: hidden;
        }
        .main {
            visibility: hidden;
        }
    }
    ```
