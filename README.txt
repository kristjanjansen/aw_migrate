Module for migrating Automatweb CMS data to Drupal

Installation:

1. Make sure you have Drush installed http://drupal.org/project/drush
2. Create empty database for Drupal
3. Create another database for Automatweb, and import existing data there
4. Download and install Drupal 7. Fastest way to do it is to run following commands:

drush dl drupal --drupal-project-rename=drupal
cd drupal
drush si --db-url=mysql://*your_database_username*:*your_database_password*@localhost/*your_drupal_database_name*

5. Add following snippet to sites/default/settings.php:

$databases['aw']['default'] = array (
  'driver' => 'mysql',
  'database' => '*your_automatweb_database_name*',
  'username' => '*your_database_username*',
  'password' => '*your_database_password*',
);

6. Run following commands inside your Drupal site root directory

git clone https://github.com/kristjanjansen/aw_migrate.git sites/all/modules/aw_migrate
drush en aw_migrate -y
drush mi --all

7. Fine-tune the migration by setting following variables:

drush vset aw_file_copy 1 -y
drush vset aw_file_path *your_automatweb_full_file_path* -y

*your_automatweb_full_web_path_to_file* looks like this: 'http://www.vonkrahl.ee/orb.aw/class=image/action=show/fastcall=1/file='

8. Add new 'field_file' for Article node type

9. Rollback and migrate again (note that this copies all your files an images over the web and can be time-consuming)

drush mr --all && drush mi --all





