<!doctype html>
<html lang="en">
	<head>
		<meta charset="utf-8">
		<title>Get Student SAP Transcript</title>
		<link rel="stylesheet" href="css/style.css">
		<link rel="stylesheet" href="css/form_style.css">
	</head>
	<body>
		<h1>Get Student SAP Transcript</h1>
		<p>You may either query by the student's First Name and Last Name, or by the student's Email.</p>
		<form action="transcript.php" method="get" id="query-transcript">
			<div class="form-container">
				<div class="row">
					<label for="FirstName" id="FirstName-ariaLabel">First Name</label>
					<input id="FirstName" name="FirstName" type="text" aria-labelledby="FirstName-ariaLabel" title="First Name" />
				</div>
				<div class="row">
					<label for="LastName" id="LastName-ariaLabel">Last Name</label>
					<input id="LastName" name="LastName" type="text" aria-labelledby="LastName-ariaLabel" title="Last Name" />
				</div>
				<div class="row">
					<label for="Email" id="Email-ariaLabel">Email</label>
					<input id="Email" name="Email" type="email" aria-labelledby="Email-ariaLabel" title="Email" />
				</div>
<div class="row">
                                        <label for="Format" id="Format-ariaLabel">Use simple format?</label>
                                        <input id="Format" name="Format" type="checkbox" aria-labelledby="Format-ariaLabel" title="Format" value="1" />
                                </div>

				<div class="row">
					<input class="form-submit-button" type="submit" value="Submit" />
				</div>
			</div>
		</form>
	</body>
</html>
